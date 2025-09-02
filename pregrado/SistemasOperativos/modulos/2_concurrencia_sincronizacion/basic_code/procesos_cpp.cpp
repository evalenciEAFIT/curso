// ================= INCLUSIONES DE LIBRERÍAS =================

// <iostream> - Proporciona funcionalidades para entrada y salida estándar en C++
// Se utiliza para mostrar información en la consola (std::cout, std::cerr, std::endl)
// y para leer datos de entrada (aunque en este programa no se usa entrada, solo salida).
#include <iostream>

// <vector> - Proporciona el contenedor std::vector, que es un array dinámico
// Se utiliza para almacenar los PIDs de los procesos hijos creados.
#include <vector>

// <string> - Proporciona la clase std::string para manejo de cadenas de caracteres
// Se utiliza para manejar nombres de recursos compartidos y mensajes de salida.
#include <string>

// <cstring> - Proporciona funciones para manipulación de cadenas de estilo C
// Se utiliza para comparar nombres de opciones en el parseo de argumentos.
#include <cstring>

// <sys/wait.h> - Proporciona declaraciones para esperar a los procesos hijos (waitpid)
// Se utiliza para que el proceso padre espere a que todos los procesos hijos terminen.
#include <sys/wait.h>

// <sys/mman.h> - Proporciona funciones para mapeo de memoria (mmap, munmap)
// Se utiliza para crear y gestionar memoria compartida entre procesos.
#include <sys/mman.h>

// <sys/stat.h> - Proporciona constantes y tipos para manejo de archivos (usado en shm_open)
// Se utiliza junto con <fcntl.h> para crear objetos de memoria compartida.
#include <sys/stat.h>

// <fcntl.h> - Proporciona constantes y funciones para control de archivos (O_CREAT, O_RDWR)
// Se utiliza para crear objetos de memoria compartida con shm_open.
#include <fcntl.h>

// <unistd.h> - Proporciona acceso a la API de POSIX (fork, close, getpid, usleep)
// Se utiliza para crear procesos (fork), obtener el PID del proceso (getpid),
// cerrar descriptores de archivo (close) y para pausar la ejecución (usleep).
#include <unistd.h>

// <semaphore.h> - Proporciona funciones para manejo de semáforos (sem_open, sem_wait, sem_post)
// Se utiliza para sincronizar el acceso a la memoria compartida entre procesos.
#include <semaphore.h>

// <chrono> - Proporciona herramientas para medición de tiempo de alta precisión
// Se utiliza para medir el tiempo de ejecución del programa y calcular métricas de rendimiento.
#include <chrono>

// <random> - Proporciona generadores de números aleatorios y distribuciones
// Se utiliza para simular cargas de trabajo variables en cada proceso.
#include <random>

// <getopt.h> - Proporciona funciones para parsear opciones de línea de comandos
// Se utiliza para procesar los argumentos pasados al programa (opciones largas y cortas).
#include <getopt.h>

// <iomanip> - Proporciona manipuladores para formato de entrada/salida
// Se utiliza para formatear la salida con precisión decimal fija (std::fixed, std::setprecision).
#include <iomanip>

// <cstdlib> - Proporciona funciones generales de la biblioteca estándar de C
// Se utiliza para la macro EXIT_SUCCESS y EXIT_FAILURE.
#include <cstdlib>

// <cstdio> - Proporciona funciones de entrada/salida de estilo C
// Se utiliza para leer el archivo /proc/self/statm para obtener el uso de memoria.
#include <cstdio>

// <csignal> - Proporciona funciones para manejo de señales
// Se utiliza para enviar señales a los procesos hijos (kill) en caso de error.
#include <csignal>

// <sstream> - Proporciona clases para streams de cadenas
// Se utiliza para formatear cadenas de manera eficiente.
#include <sstream>

// ================= DEFINICIÓN DE COLORES =================
// Códigos ANSI para colores en terminal. Permiten diferenciar visualmente
// la salida de cada proceso, facilitando la comprensión del flujo de ejecución.
const std::string COLOR_ROJO = "\033[1;31m";      // Rojo brillante
const std::string COLOR_VERDE = "\033[1;32m";     // Verde brillante
const std::string COLOR_AZUL = "\033[1;34m";      // Azul brillante
const std::string COLOR_AMARILLO = "\033[1;33m";  // Amarillo brillante
const std::string COLOR_MAGENTA = "\033[1;35m";  // Magenta brillante
const std::string COLOR_CYAN = "\033[1;36m";     // Cian brillante
const std::string COLOR_BLANCO = "\033[1;37m";  // Blanco brillante
const std::string COLOR_RESET = "\033[0m";       // Resetear color a valor por defecto

// ================= MEMORIA COMPARTIDA =================
// Estructura que contiene los datos compartidos entre todos los procesos.
// En programación con procesos, la memoria compartida es un mecanismo de IPC
// (Inter-Process Communication) que permite que múltiples procesos accedan
// a los mismos datos, pero requiere mecanismos de sincronización para evitar
// condiciones de carrera.
struct MemoriaCompartida {
    int contador_global;     // Variable compartida que todos los procesos modifican
    int iteraciones;         // Número de iteraciones para cada proceso
    bool inicializado;       // Bandera para indicar que la memoria está inicializada
};

// ================= FUNCIONES DE MEDICIÓN =================

// Función para obtener el tiempo actual en microsegundos
// Propósito: Medir el tiempo con alta precisión para evaluar rendimiento
// Cómo funciona: Usa std::chrono que proporciona alta precisión y es independiente de plataforma
long long get_time_us() {
    auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(
        now.time_since_epoch()).count();
}

// Función para obtener el uso actual de memoria en KB
// Propósito: Medir el consumo de memoria del proceso
// Cómo funciona: Lee el archivo /proc/self/statm que contiene información de memoria
long get_memory_usage() {
    FILE* file = fopen("/proc/self/statm", "r");
    if (file == nullptr) {
        return -1;
    }
    
    long size, resident, share, text, lib, data, dt;
    if (fscanf(file, "%ld %ld %ld %ld %ld %ld %ld", 
               &size, &resident, &share, &text, &lib, &data, &dt) != 7) {
        fclose(file);
        return -1;
    }
    
    fclose(file);
    // resident es el número de páginas residentes en memoria
    // Multiplicamos por el tamaño de página (generalmente 4KB)
    return resident * sysconf(_SC_PAGESIZE) / 1024;
}

// Función para formatear bytes a unidades legibles (B, KB, MB, GB)
// Propósito: Convertir valores crudos de memoria a formato humano
// Cómo funciona: Divide el valor por 1024 hasta encontrar la unidad adecuada
std::string format_bytes(long bytes) {
    const char* units[] = {"B", "KB", "MB", "GB"};
    int unit = 0;
    double size = static_cast<double>(bytes);
    
    while (size >= 1024 && unit < 3) {
        size /= 1024;
        unit++;
    }
    
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << size << " " << units[unit];
    return oss.str();
}

// Función para formatear microsegundos a unidades legibles (μs, ms, s)
// Propósito: Convertir tiempo crudo a formato humano
// Cómo funciona: Determina la unidad adecuada según la magnitud
std::string format_time_us(long long us) {
    std::ostringstream oss;
    
    if (us < 1000) {
        oss << us << " μs";
    } else if (us < 1000000) {
        oss << std::fixed << std::setprecision(2) << (us / 1000.0) << " ms";
    } else {
        oss << std::fixed << std::setprecision(2) << (us / 1000000.0) << " s";
    }
    
    return oss.str();
}

// ================= FUNCIÓN DEL PROCESO HIJO =================
// Función que ejecutará cada proceso hijo.
// Propósito: Realizar tareas concurrentes con acceso controlado a recursos compartidos
// Cómo funciona: Cada proceso ejecuta un bucle de iteraciones, simulando trabajo
//               y accediendo a memoria compartida de forma segura usando semáforos.
void funcion_proceso(int id_proceso, const std::string& color, MemoriaCompartida* memoria, 
                    sem_t* semaforo, bool verbose) {
    int contador_local = 0;
    
    // Esperar a que la memoria compartida esté inicializada
    while (!memoria->inicializado) {
        usleep(1000); // Esperar 1ms antes de volver a verificar
    }
    
    // ================= GENERADOR ALEATORIO =================
    // Cada proceso necesita su propio generador de números aleatorios con una
    // semilla única para evitar que todos generen la misma secuencia.
    // Propósito: Simular cargas de trabajo variables
    std::random_device rd;
    std::mt19937 gen(rd() + id_proceso);
    std::uniform_int_distribution<> dist(0, 2);
    
    if (verbose) {
        std::cout << color << "Proceso " << id_proceso 
                  << ": Iniciando con " << memoria->iteraciones 
                  << " iteraciones (PID: " << getpid() << ")" 
                  << COLOR_RESET << std::endl;
    }
    
    // ================= BUCLE DE TRABAJO =================
    // Cada proceso realiza 'n' iteraciones donde simula trabajo y accede
    // a recursos compartidos de forma segura.
    // Propósito: Simular un procesamiento concurrente con carga variable
    for (int i = 0; i < memoria->iteraciones; i++) {
        // ================= SIMULACIÓN DE TRABAJO =================
        // Simula una tarea que lleva tiempo variable (0-1 segundo).
        // En aplicaciones reales, esto podría ser un cálculo complejo,
        // una operación de E/S, etc.
        int carga = dist(gen);
        
        if (carga == 0) {
            // Tarea ligera: solo cálculo intensivo de CPU
            volatile int dummy = 0;
            for (int j = 0; j < 1000000; j++) {
                dummy += j;
            }
        } else if (carga == 1) {
            // Tarea media: cálculo y pequeña espera
            volatile int dummy = 0;
            for (int j = 0; j < 5000000; j++) {
                dummy += j;
            }
            usleep(10000); // Espera 10ms
        } else {
            // Tarea pesada: mucho cálculo y espera
            volatile int dummy = 0;
            for (int j = 0; j < 10000000; j++) {
                dummy += j;
            }
            usleep(50000); // Espera 50ms
        }
        
        // ================= SECCIÓN CRÍTICA =================
        // Aquí accedemos a recursos compartidos (contador_global).
        // ¿Qué es una sección crítica? Es una parte del código donde
        // múltiples procesos podrían interferir entre sí si se ejecutan
        // simultáneamente. Debe protegerse con mecanismos de sincronización.
        // Propósito: Garantizar la integridad de los datos compartidos
        
        // ================= SEMÁFORO =================
        // Los semáforos son mecanismos de sincronización que controlan el acceso
        // a recursos compartidos por múltiples procesos. A diferencia de los mutex,
        // los semáforos pueden ser compartidos entre procesos no relacionados.
        // ¿Por qué usar semáforo en lugar de mutex?
        // - Los mutex tradicionales están diseñados para hilos, no para procesos.
        // - Los semáforos pueden operar a nivel de sistema operativo.
        sem_wait(semaforo); // Esperar (decrementar el semáforo)
        
        // ================= MODIFICACIÓN DE DATOS COMPARTIDOS =================
        // Ahora que tenemos acceso exclusivo, podemos modificar el contador
        // global de forma segura.
        memoria->contador_global++;
        contador_local++;
        
        // ================= SALIDA FORMATEADA =================
        // Mostramos información con el color del proceso para facilitar
        // la identificación visual de qué proceso está ejecutándose.
        // Solo mostramos si está en modo verbose
        if (verbose) {
            std::cout << color << "Proceso " << id_proceso 
                      << ": Iteración " << i+1 << "/" << memoria->iteraciones 
                      << " | Contador global: " << memoria->contador_global 
                      << " | Contador local: " << contador_local 
                      << " | PID: " << getpid() << COLOR_RESET << std::endl;
        }
        
        sem_post(semaforo); // Señalizar (incrementar el semáforo)
    }
    
    if (verbose) {
        std::cout << color << "Proceso " << id_proceso 
                  << ": Finalizado. Realizó " << contador_local 
                  << " iteraciones (PID: " << getpid() << ")" 
                  << COLOR_RESET << std::endl;
    }
}

// ================= FUNCIÓN DE AYUDA =================
// Propósito: Mostrar información de uso del programa
// Cómo funciona: Imprime un mensaje formateado con las opciones disponibles
void print_usage(const std::string& program_name) {
    std::cout << "Uso: " << program_name << " [OPCIONES]\n";
    std::cout << "Opciones:\n";
    std::cout << "  -p, --procesos NUM_P  Número de procesos a crear (obligatorio)\n";
    std::cout << "  -i, --iteraciones NUM_I Número de iteraciones por proceso (obligatorio)\n";
    std::cout << "  -v, --verbose         Mostrar información detallada de las iteraciones\n";
    std::cout << "  --help               Mostrar este mensaje de ayuda\n";
    std::cout << "\nEjemplos:\n";
    std::cout << "  " << program_name << " --procesos 4 --iteraciones 5\n";
    std::cout << "  " << program_name << " -p 6 -i 10 -v\n";
}

// ================= FUNCIÓN PRINCIPAL =================
// Propósito: Punto de entrada del programa. Configura y ejecuta los procesos,
//            procesa argumentos de línea de comandos y muestra resultados.
int main(int argc, char* argv[]) {
    int num_procesos = 0;     // Número de procesos a crear
    int n = 0;                 // Número de iteraciones por proceso
    bool verbose = false;      // Bandera para modo verbose
    
    // Estructura para opciones largas de getopt_long
    struct option long_options[] = {
        {"procesos", required_argument, 0, 'p'},    // Opción --procesos con argumento requerido
        {"iteraciones", required_argument, 0, 'i'}, // Opción --iteraciones con argumento requerido
        {"verbose", no_argument, 0, 'v'},          // Opción --verbose sin argumento
        {"help", no_argument, 0, 0},               // Opción --help sin argumento
        {0, 0, 0, 0}                             // Marca de fin de opciones
    };
    
    // Parsear opciones de línea de comandos
    int option_index = 0;    // Índice de la opción actual
    int c;                    // Carácter de opción actual
    
    // Bucle para procesar todas las opciones
    while ((c = getopt_long(argc, argv, "p:i:v", long_options, &option_index)) != -1) {
        switch (c) {
            case 'p': // Opción -p o --procesos
                num_procesos = std::stoi(optarg); // Convertir argumento a entero
                break;
            case 'i': // Opción -i o --iteraciones
                n = std::stoi(optarg); // Convertir argumento a entero
                break;
            case 'v': // Opción -v o --verbose
                verbose = true; // Activar modo verbose
                break;
            case 0: // Opción larga sin correspondiente corta (solo --help)
                if (std::string(long_options[option_index].name) == "help") {
                    print_usage(argv[0]); // Mostrar ayuda
                    return EXIT_SUCCESS; // Salir con éxito
                }
                break;
            case '?': // Opción no reconocida
                print_usage(argv[0]); // Mostrar ayuda
                return EXIT_FAILURE; // Salir con error
            default:
                abort(); // Abortar en caso de error inesperado
        }
    }
    
    // Validar argumentos obligatorios
    // Propósito: Asegurar que el usuario proporcione los parámetros necesarios
    if (num_procesos <= 0 || n <= 0) {
        std::cerr << "Error: Debes especificar el número de procesos e iteraciones\n\n";
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }
    
    // Limitar el número máximo de procesos para evitar problemas de rendimiento
    if (num_procesos > 20) {
        std::cerr << "Error: El número máximo de procesos es 20\n";
        return EXIT_FAILURE;
    }
    
    // ================= MEDICIONES INICIALES =================
    // Propósito: Establecer una línea base para medir el rendimiento
    long long tiempo_inicio = get_time_us();       // Tiempo inicial en microsegundos
    long memoria_inicio = get_memory_usage();      // Memoria inicial en KB
    
    // Mostrar mediciones iniciales
    std::cout << COLOR_CYAN << "=== MEDICIONES INICIALES ===" << COLOR_RESET << std::endl;
    std::cout << "Tiempo inicio: " << format_time_us(tiempo_inicio) << std::endl;
    std::cout << "Memoria inicio: " << format_bytes(memoria_inicio * 1024) << std::endl;
    std::cout << std::endl;
    
    // ================= CREACIÓN DE MEMORIA COMPARTIDA =================
    // Propósito: Crear un área de memoria que pueda ser accedida por múltiples procesos
    // Cómo funciona: Usamos shm_open() para crear un objeto de memoria compartida y
    //               mmap() para mapearlo en el espacio de direcciones del proceso.
    
    // Generar un nombre único para la memoria compartida usando el PID del proceso
    std::string shm_name = "/mem_compartida_" + std::to_string(getpid());
    
    // Crear el objeto de memoria compartida
    int shm_fd = shm_open(shm_name.c_str(), O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("Error al crear memoria compartida");
        return EXIT_FAILURE;
    }
    
    // Configurar el tamaño de la memoria compartida
    if (ftruncate(shm_fd, sizeof(MemoriaCompartida)) == -1) {
        perror("Error al configurar tamaño de memoria compartida");
        close(shm_fd);
        shm_unlink(shm_name.c_str());
        return EXIT_FAILURE;
    }
    
    // Mapear la memoria compartida en el espacio de direcciones del proceso
    MemoriaCompartida* memoria = static_cast<MemoriaCompartida*>(
        mmap(nullptr, sizeof(MemoriaCompartida), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));
    
    if (memoria == MAP_FAILED) {
        perror("Error al mapear memoria compartida");
        close(shm_fd);
        shm_unlink(shm_name.c_str());
        return EXIT_FAILURE;
    }
    
    // ================= CREACIÓN DE SEMÁFORO =================
    // Propósito: Crear un semáforo para sincronizar el acceso a la memoria compartida
    // Cómo funciona: Usamos sem_open() para crear un semáforo con nombre que puede
    //               ser compartido entre procesos.
    
    // Generar un nombre único para el semáforo usando el PID del proceso
    std::string sem_name = "/semaforo_" + std::to_string(getpid());
    
    // Crear el semáforo con valor inicial 1 (binario, similar a un mutex)
    sem_t* semaforo = sem_open(sem_name.c_str(), O_CREAT, 0666, 1);
    if (semaforo == SEM_FAILED) {
        perror("Error al crear semáforo");
        munmap(memoria, sizeof(MemoriaCompartida));
        close(shm_fd);
        shm_unlink(shm_name.c_str());
        return EXIT_FAILURE;
    }
    
    // ================= INICIALIZACIÓN DE MEMORIA COMPARTIDA =================
    // Propósito: Preparar las estructuras de datos compartidos
    memoria->contador_global = 0;
    memoria->iteraciones = n;
    memoria->inicializado = false; // Aún no está inicializado para los hijos
    
    // Vector para almacenar los PIDs de los procesos hijos
    std::vector<pid_t> pids;
    pids.reserve(num_procesos);
    
    // Vector de colores disponibles para los procesos
    std::vector<std::string> colores = {
        COLOR_ROJO, COLOR_VERDE, COLOR_AZUL, COLOR_AMARILLO, 
        COLOR_MAGENTA, COLOR_CYAN, COLOR_BLANCO
    };
    
    // Mostrar información de configuración
    std::cout << "Creando " << num_procesos << " procesos con " << n 
              << " iteraciones cada uno..." << std::endl;
    if (!verbose) {
        std::cout << "Ejecutando en modo silencioso. Usa --verbose para ver detalles." 
                  << std::endl << std::endl;
    } else {
        std::cout << std::endl;
    }
    
    // ================= CREACIÓN DE PROCESOS =================
    // Propósito: Crear e iniciar los procesos que ejecutarán las tareas concurrentes
    // Cómo funciona: Usamos fork() para crear nuevos procesos. Cada proceso hijo
    //               ejecutará la misma función pero con datos diferentes.
    for (int i = 0; i < num_procesos; i++) {
        pid_t pid = fork(); // Crear un nuevo proceso
        
        if (pid == -1) {
            // Error al crear el proceso
            perror("Error al crear proceso");
            // Liberar recursos antes de salir
            for (auto p : pids) {
                kill(p, SIGTERM); // Terminar procesos hijos ya creados
                waitpid(p, nullptr, 0); // Esperar a que terminen
            }
            sem_close(semaforo);
            sem_unlink(sem_name.c_str());
            munmap(memoria, sizeof(MemoriaCompartida));
            close(shm_fd);
            shm_unlink(shm_name.c_str());
            return EXIT_FAILURE;
        } else if (pid == 0) {
            // Proceso hijo
            // Cerrar el descriptor de archivo de memoria compartida en el hijo
            // (no es necesario ya que tenemos el mapeo)
            close(shm_fd);
            
            // Ejecutar la función del proceso hijo
            funcion_proceso(i, colores[i % colores.size()], memoria, semaforo, verbose);
            
            // El proceso hijo termina aquí
            exit(EXIT_SUCCESS);
        } else {
            // Proceso padre
            pids.push_back(pid); // Guardar el PID del hijo
        }
    }
    
    // ================= INICIALIZIZACIÓN COMPLETA =================
    // Ahora que todos los procesos están creados, marcamos la memoria como inicializada
    memoria->inicializado = true;
    
    // ================= ESPERA A LOS PROCESOS HIJOS =================
    // Propósito: Sincronizar el proceso principal con los procesos secundarios
    // ¿Por qué? Si el main termina antes que los hijos, los procesos hijos pueden
    // convertirse en huérfanos y ser adoptados por el proceso init.
    for (pid_t pid : pids) {
        int status;
        waitpid(pid, &status, 0); // Esperar a que cada proceso hijo termine
    }
    
    // ================= MEDICIONES FINALES =================
    // Propósito: Obtener mediciones después de completar todo el trabajo
    long long tiempo_fin = get_time_us();       // Tiempo final en microsegundos
    long memoria_fin = get_memory_usage();      // Memoria final en KB
    
    // Calcular diferencias entre mediciones finales e iniciales
    long long tiempo_total = tiempo_fin - tiempo_inicio;  // Tiempo total ejecutado
    long memoria_usada = memoria_fin - memoria_inicio;    // Memoria consumida
    
    // ================= RESUMEN DE EJECUCIÓN =================
    // Propósito: Verificar que todo funcionó correctamente mostrando resultados
    std::cout << "\n" << COLOR_CYAN << "=== RESUMEN DE EJECUCIÓN ===" << COLOR_RESET << std::endl;
    std::cout << "Contador global final: " << memoria->contador_global << std::endl;
    std::cout << "Esperado: " << num_procesos * n << " (" << num_procesos << " procesos × " 
              << n << " iteraciones)" << std::endl << std::endl;
    
    // ================= MEDICIONES FINALES =================
    std::cout << COLOR_CYAN << "=== MEDICIONES FINALES ===" << COLOR_RESET << std::endl;
    std::cout << "Tiempo fin: " << format_time_us(tiempo_fin) << std::endl;
    std::cout << "Memoria fin: " << format_bytes(memoria_fin * 1024) << std::endl;
    std::cout << std::endl;
    
    // ================= DIFERENCIAS DE MEDICIONES =================
    // Propósito: Mostrar el consumo real de recursos durante la ejecución
    std::cout << COLOR_CYAN << "=== DIFERENCIAS DE MEDICIONES ===" << COLOR_RESET << std::endl;
    std::cout << "Tiempo total: " << format_time_us(tiempo_total) << std::endl;
    std::cout << "Memoria utilizada: " << format_bytes(memoria_usada * 1024) << std::endl;
    
    // ================= MÉTRICAS DE RENDIMIENTO =================
    // Propósito: Calcular y mostrar indicadores de rendimiento del programa
    std::cout << "\n" << COLOR_CYAN << "=== MÉTRICAS DE RENDIMIENTO ===" << COLOR_RESET << std::endl;
    double operaciones = num_procesos * n;  // Total de operaciones realizadas
    double segundos = tiempo_total / 1000000.0;  // Tiempo total en segundos
    double ops_por_segundo = operaciones / segundos;  // Operaciones por segundo
    
    std::cout << "Operaciones totales: " << operaciones << std::endl;
    std::cout << "Operaciones por segundo: " << std::fixed << std::setprecision(2) 
              << ops_por_segundo << " ops/s" << std::endl;
    
    // ================= LIMPIEZA =================
    // Propósito: Liberar todos los recursos asignados antes de terminar
    
    // Cerrar el semáforo y eliminarlo
    sem_close(semaforo);
    sem_unlink(sem_name.c_str());
    
    // Desmapear la memoria compartida
    munmap(memoria, sizeof(MemoriaCompartida));
    
    // Cerrar el descriptor de archivo de memoria compartida
    close(shm_fd);
    
    // Eliminar el objeto de memoria compartida
    shm_unlink(shm_name.c_str());
    
    return EXIT_SUCCESS; // Salir con éxito
}