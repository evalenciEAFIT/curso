#include <iostream>     // Para entrada/salida estándar (cout, cin, cerr)
#include <thread>       // Para manejo de hilos (std::thread)
#include <vector>       // Para contenedor dinámico (std::vector)
#include <mutex>        // Para mecanismos de sincronización (std::mutex)
#include <chrono>       // Para mediciones de tiempo (std::chrono)
#include <random>       // Para generación de números aleatorios
#include <string>       // Para manejo de cadenas (std::string)
#include <cstdlib>      // Para funciones estándar (atoi, exit)
#include <getopt.h>     // Para parsear opciones de línea de comandos
#include <unistd.h>     // Para funciones POSIX (usleep)
#include <sys/time.h>   // Para medición de tiempo (gettimeofday)
#include <sys/resource.h> // Para obtener uso de recursos (getrusage)
#include <iomanip>      // Para manipuladores de formato (std::fixed, std::setprecision)

// ================= DEFINICIÓN DE COLORES =================
// Códigos ANSI para colores en terminal. Permiten diferenciar visualmente
// la salida de cada hilo, facilitando la comprensión del flujo de ejecución.
const std::string COLOR_ROJO = "\033[1;31m";      // Rojo brillante
const std::string COLOR_VERDE = "\033[1;32m";     // Verde brillante
const std::string COLOR_AZUL = "\033[1;34m";      // Azul brillante
const std::string COLOR_AMARILLO = "\033[1;33m";  // Amarillo brillante
const std::string COLOR_MAGENTA = "\033[1;35m";  // Magenta brillante
const std::string COLOR_CYAN = "\033[1;36m";     // Cian brillante
const std::string COLOR_BLANCO = "\033[1;37m";  // Blanco brillante
const std::string COLOR_RESET = "\033[0m";       // Resetear color a valor por defecto

// ================= MEMORIA COMPARTIDA =================
// Estructura que contiene los datos compartidos entre todos los hilos.
// En programación concurrente, la memoria compartida permite que múltiples
// hilos accedan a los mismos datos, pero requiere mecanismos de sincronización
// para evitar condiciones de carrera.
struct MemoriaCompartida {
    int contador_global = 0;  // Variable compartida que todos los hilos modifican
    std::mutex mutex;          // Mutex para proteger el acceso concurrente
    int iteraciones;           // Número de iteraciones para cada hilo
    
    // ¿Por qué usar un mutex aquí?
    // Sin sincronización, múltiples hilos podrían modificar 'contador_global'
    // simultáneamente, causando una condición de carrera donde el valor final
    // sería impredecible. El mutex asegura que solo un hilo acceda al recurso
    // a la vez (exclusión mutua).
};

// ================= DATOS DEL HILO =================
// Estructura que contiene información específica para cada hilo.
// Cada hilo necesita sus propios datos locales además de acceder a la memoria compartida.
struct DatosHilo {
    int id_hilo;              // Identificador único del hilo
    std::string color;        // Color ANSI para la salida en terminal
    MemoriaCompartida& memoria; // Referencia a la memoria compartida
    bool verbose;             // Bandera para mostrar iteraciones
    
    // Constructor para inicializar los datos del hilo
    DatosHilo(int id, const std::string& col, MemoriaCompartida& mem, bool verb)
        : id_hilo(id), color(col), memoria(mem), verbose(verb) {}
};

// ================= FUNCIONES DE MEDICIÓN =================

// Función para obtener el tiempo actual en microsegundos
// Propósito: Medir el tiempo con alta precisión para evaluar rendimiento
// Cómo funciona: Usa gettimeofday() que devuelve tiempo con resolución de microsegundos
long long get_time_us() {
    struct timeval tv;       // Estructura para almacenar tiempo (segundos y microsegundos)
    gettimeofday(&tv, NULL); // Obtiene tiempo actual del sistema
    // Combina segundos y microsegundos en un solo valor en microsegundos
    return (long long)tv.tv_sec * 1000000 + tv.tv_usec;
}

// Función para obtener el uso actual de memoria en KB
// Propósito: Medir el consumo de memoria del proceso
// Cómo funciona: Usa getrusage() que obtiene estadísticas de uso de recursos
long get_memory_usage() {
    struct rusage usage;     // Estructura para almacenar estadísticas de recursos
    getrusage(RUSAGE_SELF, &usage); // Obtiene uso de recursos del proceso actual
    return usage.ru_maxrss;  // Retorna el máximo uso de memoria residente en KB
}

// Función para formatear bytes a unidades legibles (B, KB, MB, GB)
// Propósito: Convertir valores crudos de memoria a formato humano
// Cómo funciona: Divide el valor por 1024 hasta encontrar la unidad adecuada
std::string format_bytes(long bytes) {
    const char* units[] = {"B", "KB", "MB", "GB"}; // Unidades de medida
    int unit = 0;            // Índice de unidad actual (empezando en Bytes)
    double size = static_cast<double>(bytes); // Tamaño en bytes como double para divisiones
    
    // Convertimos a la unidad más grande posible mientras sea >= 1024
    while (size >= 1024 && unit < 3) {
        size /= 1024;        // Dividimos por 1024 para pasar a la siguiente unidad
        unit++;              // Incrementamos el índice de unidad
    }
    
    // Creamos un stream para formatear el resultado
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
        // Si es menor a 1000 μs, lo dejamos en microsegundos
        oss << us << " μs";
    } else if (us < 1000000) {
        // Si está entre 1000 μs y 1 segundo, lo convertimos a milisegundos
        oss << std::fixed << std::setprecision(2) << (us / 1000.0) << " ms";
    } else {
        // Si es mayor o igual a 1 segundo, lo convertimos a segundos
        oss << std::fixed << std::setprecision(2) << (us / 1000000.0) << " s";
    }
    
    return oss.str();
}

// ================= FUNCIÓN DEL HILO =================
// Función que ejecutará cada hilo. Demuestra el uso de memoria compartida
// y sincronización en un entorno multihilo.
// Propósito: Realizar tareas concurrentes con acceso controlado a recursos compartidos
// Cómo funciona: Cada hilo ejecuta un bucle de iteraciones, simulando trabajo
//               y accediendo a memoria compartida de forma segura.
void funcion_hilo(DatosHilo datos) {
    int contador_local = 0;    // Contador local (no compartido) para cada hilo
    
    // ================= GENERADOR ALEATORIO =================
    // Cada hilo necesita su propio generador de números aleatorios con una
    // semilla única para evitar que todos generen la misma secuencia.
    // Propósito: Simular cargas de trabajo variables
    std::random_device rd;                    // Fuente de entropía del hardware
    std::mt19937 gen(rd() + datos.id_hilo);   // Motor Mersenne Twister
    std::uniform_int_distribution<> dist(0, 2); // Distribución uniforme (0-2)
    
    // Si está en modo verbose, mostramos mensaje de inicio
    if (datos.verbose) {
        std::cout << datos.color << "Hilo " << datos.id_hilo 
                  << ": Iniciando con " << datos.memoria.iteraciones 
                  << " iteraciones" << COLOR_RESET << std::endl;
    }
    
    // ================= BUCLE DE TRABAJO =================
    // Cada hilo realiza 'n' iteraciones donde simula trabajo y accede
    // a recursos compartidos de forma segura.
    // Propósito: Simular un procesamiento concurrente con carga variable
    for (int i = 0; i < datos.memoria.iteraciones; i++) {
        // ================= SIMULACIÓN DE TRABAJO =================
        // Simula una tarea que lleva tiempo variable (0-1 segundo).
        // En aplicaciones reales, esto podría ser un cálculo complejo,
        // una operación de E/S, etc.
        int carga = dist(gen); // Obtenemos un número aleatorio entre 0 y 2
        
        if (carga == 0) {
            // Tarea ligera: solo cálculo intensivo de CPU
            volatile int dummy = 0; // volatile para evitar optimización del compilador
            for (int j = 0; j < 1000000; j++) {
                dummy += j;        // Operación simple que consume CPU
            }
        } else if (carga == 1) {
            // Tarea media: cálculo y pequeña espera
            volatile int dummy = 0;
            for (int j = 0; j < 5000000; j++) {
                dummy += j;        // Más cálculos
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Espera 10ms
        } else {
            // Tarea pesada: mucho cálculo y espera
            volatile int dummy = 0;
            for (int j = 0; j < 10000000; j++) {
                dummy += j;        // Aún más cálculos
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Espera 50ms
        }
        
        // ================= SECCIÓN CRÍTICA =================
        // Aquí accedemos a recursos compartidos (contador_global).
        // ¿Qué es una sección crítica? Es una parte del código donde
        // múltiples hilos podrían interferir entre sí si se ejecutan
        // simultáneamente. Debe protegerse con mecanismos de sincronización.
        // Propósito: Garantizar la integridad de los datos compartidos
        {
            // ================= LOCK_GUARD =================
            // std::lock_guard es un RAII (Resource Acquisition Is Initialization)
            // para mutexes. Adquiere el mutex al crearse y lo libera automáticamente
            // al salir del scope (al final del bloque {}).
            // ¿Por qué es mejor que lock/unlock manual?
            // - Excepción seguro: Si ocurre una excepción, el mutex se libera.
            // - Previene olvidos: No hay que recordar desbloquear manualmente.
            std::lock_guard<std::mutex> lock(datos.memoria.mutex);
            
            // ================= MODIFICACIÓN DE DATOS COMPARTIDOS =================
            // Ahora que tenemos acceso exclusivo, podemos modificar el contador
            // global de forma segura.
            datos.memoria.contador_global++; // Incrementamos contador compartido
            contador_local++;                // Incrementamos contador local (no necesita protección)
            
            // ================= SALIDA FORMATEADA =================
            // Mostramos información con el color del hilo para facilitar
            // la identificación visual de qué hilo está ejecutándose.
            // Solo mostramos si está en modo verbose
            if (datos.verbose) {
                std::cout << datos.color << "Hilo " << datos.id_hilo 
                          << ": Iteración " << i+1 << "/" << datos.memoria.iteraciones 
                          << " | Contador global: " << datos.memoria.contador_global 
                          << " | Contador local: " << contador_local 
                          << COLOR_RESET << std::endl;
            }
        }
        // Al salir del bloque, lock_guard libera automáticamente el mutex,
        // permitiendo que otros hilos entren a la sección crítica.
    }
    
    // Si está en modo verbose, mostramos mensaje de finalización
    if (datos.verbose) {
        std::cout << datos.color << "Hilo " << datos.id_hilo 
                  << ": Finalizado. Realizó " << contador_local 
                  << " iteraciones" << COLOR_RESET << std::endl;
    }
}

// ================= FUNCIÓN DE AYUDA =================
// Propósito: Mostrar información de uso del programa
// Cómo funciona: Imprime un mensaje formateado con las opciones disponibles
void print_usage(const std::string& program_name) {
    std::cout << "Uso: " << program_name << " [OPCIONES]\n";
    std::cout << "Opciones:\n";
    std::cout << "  -h, --hilos NUM_H     Número de hilos a crear (obligatorio)\n";
    std::cout << "  -i, --iteraciones NUM_I Número de iteraciones por hilo (obligatorio)\n";
    std::cout << "  -v, --verbose         Mostrar información detallada de las iteraciones\n";
    std::cout << "  --help               Mostrar este mensaje de ayuda\n";
    std::cout << "\nEjemplos:\n";
    std::cout << "  " << program_name << " --hilos 4 --iteraciones 5\n";
    std::cout << "  " << program_name << " -h 6 -i 10 -v\n";
}

// ================= FUNCIÓN PRINCIPAL =================
// Propósito: Punto de entrada del programa. Configura y ejecuta los hilos,
//            procesa argumentos de línea de comandos y muestra resultados.
int main(int argc, char* argv[]) {
    int num_hilos = 0;        // Número de hilos a crear
    int n = 0;                // Número de iteraciones por hilo
    bool verbose = false;     // Bandera para modo verbose
    
    // Estructura para opciones largas de getopt_long
    struct option long_options[] = {
        {"hilos", required_argument, 0, 'h'},      // Opción --hilos con argumento requerido
        {"iteraciones", required_argument, 0, 'i'}, // Opción --iteraciones con argumento requerido
        {"verbose", no_argument, 0, 'v'},          // Opción --verbose sin argumento
        {"help", no_argument, 0, 0},               // Opción --help sin argumento
        {0, 0, 0, 0}                             // Marca de fin de opciones
    };
    
    // Parsear opciones de línea de comandos
    int option_index = 0;    // Índice de la opción actual
    int c;                    // Carácter de opción actual
    
    // Bucle para procesar todas las opciones
    while ((c = getopt_long(argc, argv, "h:i:v", long_options, &option_index)) != -1) {
        switch (c) {
            case 'h': // Opción -h o --hilos
                num_hilos = std::stoi(optarg); // Convertir argumento a entero
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
    if (num_hilos <= 0 || n <= 0) {
        std::cerr << "Error: Debes especificar el número de hilos e iteraciones\n\n";
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }
    
    // Limitar el número máximo de hilos para evitar problemas de rendimiento
    if (num_hilos > 20) {
        std::cerr << "Error: El número máximo de hilos es 20\n";
        return EXIT_FAILURE;
    }
    
    // ================= MEDICIONES INICIALES =================
    // Propósito: Establecer una línea base para medir el rendimiento
    long long tiempo_inicio = get_time_us();       // Tiempo inicial en microsegundos
    long memoria_inicio = get_memory_usage();      // Memoria inicial en KB
    
    // Mostrar mediciones iniciales
    std::cout << COLOR_CYAN << "=== MEDICIONES INICIALES (c++) ===" << COLOR_RESET << std::endl;
    std::cout << "Tiempo inicio: " << format_time_us(tiempo_inicio) << std::endl;
    std::cout << "Memoria inicio: " << format_bytes(memoria_inicio * 1024) << std::endl;
    std::cout << std::endl;
    
    // ================= INICIALIZACIÓN =================
    // Propósito: Preparar las estructuras de datos y recursos necesarios
    MemoriaCompartida memoria;
    memoria.iteraciones = n;      // Establecer número de iteraciones
    
    // Vector para almacenar los hilos
    std::vector<std::thread> hilos;
    hilos.reserve(num_hilos); // Reservar espacio para evitar reasignaciones
    
    // Vector de colores disponibles para los hilos
    std::vector<std::string> colores = {
        COLOR_ROJO, COLOR_VERDE, COLOR_AZUL, COLOR_AMARILLO, 
        COLOR_MAGENTA, COLOR_CYAN, COLOR_BLANCO
    };
    
    // Mostrar información de configuración
    std::cout << "Creando " << num_hilos << " hilos con " << n 
              << " iteraciones cada uno..." << std::endl;
    if (!verbose) {
        std::cout << "Ejecutando en modo silencioso. Usa --verbose para ver detalles." 
                  << std::endl << std::endl;
    } else {
        std::cout << std::endl;
    }
    
    // ================= CREACIÓN DE HILOS =================
    // Propósito: Crear e iniciar los hilos que ejecutarán las tareas concurrentes
    for (int i = 0; i < num_hilos; i++) {
        // Crear datos para el hilo
        DatosHilo datos(i, colores[i % colores.size()], memoria, verbose);
        
        // ================= EMPPLACE_BACK =================
        // Creamos el hilo y lo añadimos al vector.
        // ¿Por qué usar emplace_back en lugar de push_back?
        // - Más eficiente: Construye el objeto directamente en el vector
        //   sin copias temporales.
        // - Más flexible: Permite pasar argumentos directamente al constructor.
        hilos.emplace_back(funcion_hilo, std::move(datos));
        
        // ¿Por qué std::move(datos)?
        // - Evita copias innecesarias de la estructura DatosHilo.
        // - Permite transferir eficientemente los datos al hilo.
    }
    
    // ================= ESPERA A LOS HILOS =================
    // Propósito: Sincronizar el hilo principal con los hilos secundarios
    // ¿Por qué? Si el main termina antes que los hilos, el programa se cerrará
    // abruptamente, terminando todos los hilos antes de que completen su trabajo.
    for (auto& hilo : hilos) {
        hilo.join(); // Esperar a que cada hilo termine
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
    std::cout << "Contador global final: " << memoria.contador_global << std::endl;
    std::cout << "Esperado: " << num_hilos * n << " (" << num_hilos << " hilos × " 
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
    double operaciones = num_hilos * n;  // Total de operaciones realizadas
    double segundos = tiempo_total / 1000000.0;  // Tiempo total en segundos
    double ops_por_segundo = operaciones / segundos;  // Operaciones por segundo
    
    std::cout << "Operaciones totales: " << operaciones << std::endl;
    std::cout << "Operaciones por segundo: " << std::fixed << std::setprecision(2) 
              << ops_por_segundo << " ops/s" << std::endl;
    
    return EXIT_SUCCESS; // Salir con éxito
}