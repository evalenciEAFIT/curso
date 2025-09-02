// ================= INCLUSIONES DE LIBRERÍAS =================

// <stdio.h> - Biblioteca estándar de entrada/salida en C
// Proporciona funciones para entrada y salida como printf, fprintf, perror, etc.
// Se utiliza para mostrar información en la consola y mensajes de error.
#include <stdio.h>

// <stdlib.h> - Biblioteca estándar general en C
// Proporciona funciones para gestión de memoria (malloc, free), conversión de cadenas (atoi),
// y macros como EXIT_SUCCESS y EXIT_FAILURE.
#include <stdlib.h>

// <unistd.h> - Proporciona acceso a la API de POSIX (Portable Operating System Interface)
// Contiene funciones para operaciones de sistema como fork (crear procesos), 
// getpid (obtener ID de proceso), close (cerrar descriptores), usleep (pausar ejecución).
#include <unistd.h>

// <sys/wait.h> - Contiene declaraciones para esperar a los procesos hijos
// Proporciona la función waitpid que permite al proceso padre esperar a que los hijos terminen.
#include <sys/wait.h>

// <sys/mman.h> - Proporciona funciones para mapeo de memoria
// Contiene mmap (mapear memoria), munmap (liberar mapeo), shm_open (crear memoria compartida),
// y otras funciones para gestionar memoria compartida entre procesos.
#include <sys/mman.h>

// <sys/stat.h> - Define constantes y tipos para manejo de archivos
// Se utiliza junto con fcntl.h para crear objetos de memoria compartida con permisos adecuados.
#include <sys/stat.h>

// <fcntl.h> - Proporciona constantes y funciones para control de archivos
// Contiene constantes como O_CREAT, O_RDWR utilizadas al abrir/crear archivos y objetos compartidos.
#include <fcntl.h>

// <semaphore.h> - Proporciona funciones para manejo de semáforos
// Contiene sem_open (crear semáforo), sem_wait (esperar semáforo), sem_post (liberar semáforo),
// utilizados para sincronizar el acceso a recursos compartidos entre procesos.
#include <semaphore.h>

// <sys/time.h> - Proporciona funciones para medición de tiempo
// Contiene gettimeofday para obtener el tiempo actual con alta precisión (microsegundos).
#include <sys/time.h>

// <time.h> - Proporciona funciones para manipulación de tiempo y fechas
// Se utiliza para la función time que obtiene el tiempo actual para inicializar generadores aleatorios.
#include <time.h>

// <string.h> - Proporciona funciones para manipulación de cadenas
// Contiene strcmp para comparar cadenas, utilizado en el parseo de opciones.
#include <string.h>

// <getopt.h> - Proporciona funciones para parsear opciones de línea de comandos
// Contiene getopt_long para procesar argumentos con formato largo y corto.
#include <getopt.h>

// <signal.h> - Proporciona funciones para manejo de señales
// Contiene kill para enviar señales a procesos (por ejemplo, para terminar procesos hijos en caso de error).
#include <signal.h>

// <sys/types.h> - Define tipos de datos utilizados en llamadas al sistema
// Proporciona tipos como pid_t para identificadores de proceso.
#include <sys/types.h>

// <sys/resource.h> - Proporciona funciones para obtener uso de recursos
// Contiene getrusage para obtener estadísticas de uso de recursos como memoria.
#include <sys/resource.h>

// ================= DEFINICIÓN DE COLORES =================
// Códigos ANSI para colores en terminal. Permiten diferenciar visualmente
// la salida de cada proceso, facilitando la comprensión del flujo de ejecución.
#define COLOR_ROJO     "\033[1;31m"      // Rojo brillante
#define COLOR_VERDE    "\033[1;32m"      // Verde brillante
#define COLOR_AZUL     "\033[1;34m"      // Azul brillante
#define COLOR_AMARILLO "\033[1;33m"      // Amarillo brillante
#define COLOR_MAGENTA  "\033[1;35m"      // Magenta brillante
#define COLOR_CYAN     "\033[1;36m"      // Cian brillante
#define COLOR_BLANCO   "\033[1;37m"      // Blanco brillante
#define COLOR_RESET    "\033[0m"         // Resetear color a valor por defecto

// ================= MEMORIA COMPARTIDA =================
// Estructura que contiene los datos compartidos entre todos los procesos.
// En programación con procesos, la memoria compartida es un mecanismo de IPC
// (Inter-Process Communication) que permite que múltiples procesos accedan
// a los mismos datos, pero requiere mecanismos de sincronización para evitar
// condiciones de carrera.
typedef struct {
    int contador_global;     // Variable compartida que todos los procesos modifican
    int iteraciones;         // Número de iteraciones para cada proceso
    int inicializado;        // Bandera para indicar que la memoria está inicializada
} MemoriaCompartida;

// ================= FUNCIONES DE MEDICIÓN =================

// Función para obtener el tiempo actual en microsegundos
// Propósito: Medir el tiempo con alta precisión para evaluar rendimiento
// Cómo funciona: Usa gettimeofday que devuelve tiempo con resolución de microsegundos
long long get_time_us() {
    struct timeval tv;       // Estructura para almacenar tiempo (segundos y microsegundos)
    gettimeofday(&tv, NULL); // Obtiene tiempo actual del sistema
    // Combina segundos y microsegundos en un solo valor en microsegundos
    return (long long)tv.tv_sec * 1000000 + tv.tv_usec;
}

// Función para obtener el uso actual de memoria en KB
// Propósito: Medir el consumo de memoria del proceso
// Cómo funciona: Usa getrusage que obtiene estadísticas de uso de recursos
long get_memory_usage() {
    struct rusage usage;     // Estructura para almacenar estadísticas de recursos
    getrusage(RUSAGE_SELF, &usage); // Obtiene uso de recursos del proceso actual
    return usage.ru_maxrss;  // Retorna el máximo uso de memoria residente en KB
}

// Función para formatear bytes a unidades legibles (B, KB, MB, GB)
// Propósito: Convertir valores crudos de memoria a formato humano
// Cómo funciona: Divide el valor por 1024 hasta encontrar la unidad adecuada
void format_bytes(long bytes, char* buffer, size_t buffer_size) {
    const char* units[] = {"B", "KB", "MB", "GB"}; // Unidades de medida
    int unit = 0;            // Índice de unidad actual (empezando en Bytes)
    double size = (double)bytes; // Tamaño en bytes como double para divisiones
    
    // Convertimos a la unidad más grande posible mientras sea >= 1024
    while (size >= 1024 && unit < 3) {
        size /= 1024;        // Dividimos por 1024 para pasar a la siguiente unidad
        unit++;              // Incrementamos el índice de unidad
    }
    
    // Formateamos el resultado con 2 decimales y la unidad correspondiente
    snprintf(buffer, buffer_size, "%.2f %s", size, units[unit]);
}

// Función para formatear microsegundos a unidades legibles (μs, ms, s)
// Propósito: Convertir tiempo crudo a formato humano
// Cómo funciona: Determina la unidad adecuada según la magnitud
void format_time_us(long long us, char* buffer, size_t buffer_size) {
    if (us < 1000) {
        // Si es menor a 1000 μs, lo dejamos en microsegundos
        snprintf(buffer, buffer_size, "%lld μs", us);
    } else if (us < 1000000) {
        // Si está entre 1000 μs y 1 segundo, lo convertimos a milisegundos
        snprintf(buffer, buffer_size, "%.2f ms", us / 1000.0);
    } else {
        // Si es mayor o igual a 1 segundo, lo convertimos a segundos
        snprintf(buffer, buffer_size, "%.2f s", us / 1000000.0);
    }
}

// ================= FUNCIÓN DEL PROCESO HIJO =================
// Función que ejecutará cada proceso hijo.
// Propósito: Realizar tareas concurrentes con acceso controlado a recursos compartidos
// Cómo funciona: Cada proceso ejecuta un bucle de iteraciones, simulando trabajo
//               y accediendo a memoria compartida de forma segura usando semáforos.
void funcion_proceso(int id_proceso, const char* color, MemoriaCompartida* memoria, 
                    sem_t* semaforo, int verbose) {
    int contador_local = 0;    // Contador local (no compartido) para cada proceso
    
    // Esperar a que la memoria compartida esté inicializada
    while (!memoria->inicializado) {
        usleep(1000); // Esperar 1ms antes de volver a verificar
    }
    
    // ================= GENERADOR ALEATORIO =================
    // Cada proceso necesita su propio generador de números aleatorios con una
    // semilla única para evitar que todos generen la misma secuencia.
    // Propósito: Simular cargas de trabajo variables
    unsigned int seed = time(NULL) + id_proceso; // Semilla única (tiempo + ID del proceso)
    srand(seed);             // Inicializamos el generador
    
    if (verbose) {
        printf("%sProceso %d: Iniciando con %d iteraciones (PID: %d)%s\n", 
               color, id_proceso, memoria->iteraciones, getpid(), COLOR_RESET);
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
        int carga = rand() % 3; // Obtenemos un número aleatorio entre 0 y 2
        
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
            usleep(10000);         // Espera 10ms (10000 microsegundos)
        } else {
            // Tarea pesada: mucho cálculo y espera
            volatile int dummy = 0;
            for (int j = 0; j < 10000000; j++) {
                dummy += j;        // Aún más cálculos
            }
            usleep(50000);         // Espera 50ms (50000 microsegundos)
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
        memoria->contador_global++; // Incrementamos contador compartido
        contador_local++;          // Incrementamos contador local (no necesita protección)
        
        // ================= SALIDA FORMATEADA =================
        // Mostramos información con el color del proceso para facilitar
        // la identificación visual de qué proceso está ejecutándose.
        // Solo mostramos si está en modo verbose
        if (verbose) {
            printf("%sProceso %d: Iteración %d/%d | Contador global: %d | Contador local: %d | PID: %d%s\n",
                   color, id_proceso, i+1, memoria->iteraciones, 
                   memoria->contador_global, contador_local, getpid(), COLOR_RESET);
        }
        
        sem_post(semaforo); // Señalizar (incrementar el semáforo)
    }
    
    if (verbose) {
        printf("%sProceso %d: Finalizado. Realizó %d iteraciones (PID: %d)%s\n", 
               color, id_proceso, contador_local, getpid(), COLOR_RESET);
    }
}

// ================= FUNCIÓN DE AYUDA =================
// Propósito: Mostrar información de uso del programa
// Cómo funciona: Imprime un mensaje formateado con las opciones disponibles
void print_usage(const char* program_name) {
    printf("Uso: %s [OPCIONES]\n", program_name);
    printf("Opciones:\n");
    printf("  -p, --procesos NUM_P  Número de procesos a crear (obligatorio)\n");
    printf("  -i, --iteraciones NUM_I Número de iteraciones por proceso (obligatorio)\n");
    printf("  -v, --verbose         Mostrar información detallada de las iteraciones\n");
    printf("  --help               Mostrar este mensaje de ayuda\n");
    printf("\nEjemplos:\n");
    printf("  %s --procesos 4 --iteraciones 5\n", program_name);
    printf("  %s -p 6 -i 10 -v\n", program_name);
}

// ================= FUNCIÓN PRINCIPAL =================
// Propósito: Punto de entrada del programa. Configura y ejecuta los procesos,
//            procesa argumentos de línea de comandos y muestra resultados.
int main(int argc, char* argv[]) {
    int num_procesos = 0;     // Número de procesos a crear
    int n = 0;                 // Número de iteraciones por proceso
    int verbose = 0;           // Bandera para modo verbose (0=falso, 1=verdadero)
    
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
                num_procesos = atoi(optarg); // Convertir argumento a entero
                break;
            case 'i': // Opción -i o --iteraciones
                n = atoi(optarg); // Convertir argumento a entero
                break;
            case 'v': // Opción -v o --verbose
                verbose = 1; // Activar modo verbose
                break;
            case 0: // Opción larga sin correspondiente corta (solo --help)
                if (strcmp(long_options[option_index].name, "help") == 0) {
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
        fprintf(stderr, "Error: Debes especificar el número de procesos e iteraciones\n\n");
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }
    
    // Limitar el número máximo de procesos para evitar problemas de rendimiento
    if (num_procesos > 20) {
        fprintf(stderr, "Error: El número máximo de procesos es 20\n");
        return EXIT_FAILURE;
    }
    
    // ================= MEDICIONES INICIALES =================
    // Propósito: Establecer una línea base para medir el rendimiento
    long long tiempo_inicio = get_time_us();       // Tiempo inicial en microsegundos
    long memoria_inicio = get_memory_usage();      // Memoria inicial en KB
    
    // Buffers para formatear las mediciones
    char tiempo_buf[32], memoria_buf[32];
    
    // Mostrar mediciones iniciales
    printf(COLOR_CYAN "=== MEDICIONES INICIALES ===" COLOR_RESET "\n");
    format_time_us(tiempo_inicio, tiempo_buf, sizeof(tiempo_buf));
    printf("Tiempo inicio: %s\n", tiempo_buf);
    
    format_bytes(memoria_inicio * 1024, memoria_buf, sizeof(memoria_buf));
    printf("Memoria inicio: %s\n", memoria_buf);
    printf("\n");
    
    // ================= CREACIÓN DE MEMORIA COMPARTIDA =================
    // Propósito: Crear un área de memoria que pueda ser accedida por múltiples procesos
    // Cómo funciona: Usamos shm_open para crear un objeto de memoria compartida y
    //               mmap para mapearlo en el espacio de direcciones del proceso.
    
    // Generar un nombre único para la memoria compartida usando el PID del proceso
    char shm_name[256];
    snprintf(shm_name, sizeof(shm_name), "/mem_compartida_%d", getpid());
    
    // Crear el objeto de memoria compartida
    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("Error al crear memoria compartida");
        return EXIT_FAILURE;
    }
    
    // Configurar el tamaño de la memoria compartida
    if (ftruncate(shm_fd, sizeof(MemoriaCompartida)) == -1) {
        perror("Error al configurar tamaño de memoria compartida");
        close(shm_fd);
        shm_unlink(shm_name);
        return EXIT_FAILURE;
    }
    
    // Mapear la memoria compartida en el espacio de direcciones del proceso
    MemoriaCompartida* memoria = (MemoriaCompartida*)mmap(NULL, sizeof(MemoriaCompartida),
                                                               PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    
    if (memoria == MAP_FAILED) {
        perror("Error al mapear memoria compartida");
        close(shm_fd);
        shm_unlink(shm_name);
        return EXIT_FAILURE;
    }
    
    // ================= CREACIÓN DE SEMÁFORO =================
    // Propósito: Crear un semáforo para sincronizar el acceso a la memoria compartida
    // Cómo funciona: Usamos sem_open para crear un semáforo con nombre que puede
    //               ser compartido entre procesos.
    
    // Generar un nombre único para el semáforo usando el PID del proceso
    char sem_name[256];
    snprintf(sem_name, sizeof(sem_name), "/semaforo_%d", getpid());
    
    // Crear el semáforo con valor inicial 1 (binario, similar a un mutex)
    sem_t* semaforo = sem_open(sem_name, O_CREAT, 0666, 1);
    if (semaforo == SEM_FAILED) {
        perror("Error al crear semáforo");
        munmap(memoria, sizeof(MemoriaCompartida));
        close(shm_fd);
        shm_unlink(shm_name);
        return EXIT_FAILURE;
    }
    
    // ================= INICIALIZACIÓN DE MEMORIA COMPARTIDA =================
    // Propósito: Preparar las estructuras de datos compartidos
    memoria->contador_global = 0;
    memoria->iteraciones = n;
    memoria->inicializado = 0; // Aún no está inicializado para los hijos
    
    // Array para almacenar los PIDs de los procesos hijos
    pid_t* pids = malloc(sizeof(pid_t) * num_procesos);
    if (pids == NULL) {
        perror("Error al asignar memoria para los PIDs");
        sem_close(semaforo);
        sem_unlink(sem_name);
        munmap(memoria, sizeof(MemoriaCompartida));
        close(shm_fd);
        shm_unlink(shm_name);
        return EXIT_FAILURE;
    }
    
    // Array de colores disponibles para los procesos
    const char* colores[] = {
        COLOR_ROJO, COLOR_VERDE, COLOR_AZUL, COLOR_AMARILLO, 
        COLOR_MAGENTA, COLOR_CYAN, COLOR_BLANCO
    };
    int num_colores = sizeof(colores) / sizeof(colores[0]);
    
    // Mostrar información de configuración
    printf("Creando %d procesos con %d iteraciones cada uno...\n", num_procesos, n);
    if (!verbose) {
        printf("Ejecutando en modo silencioso. Usa --verbose para ver detalles.\n\n");
    } else {
        printf("\n");
    }
    
    // ================= CREACIÓN DE PROCESOS =================
    // Propósito: Crear e iniciar los procesos que ejecutarán las tareas concurrentes
    // Cómo funciona: Usamos fork para crear nuevos procesos. Cada proceso hijo
    //               ejecutará la misma función pero con datos diferentes.
    for (int i = 0; i < num_procesos; i++) {
        pid_t pid = fork(); // Crear un nuevo proceso
        
        if (pid == -1) {
            // Error al crear el proceso
            perror("Error al crear proceso");
            // Liberar recursos antes de salir
            for (int j = 0; j < i; j++) {
                kill(pids[j], SIGTERM); // Terminar procesos hijos ya creados
                waitpid(pids[j], NULL, 0); // Esperar a que terminen
            }
            free(pids);
            sem_close(semaforo);
            sem_unlink(sem_name);
            munmap(memoria, sizeof(MemoriaCompartida));
            close(shm_fd);
            shm_unlink(shm_name);
            return EXIT_FAILURE;
        } else if (pid == 0) {
            // Proceso hijo
            // Cerrar el descriptor de archivo de memoria compartida en el hijo
            // (no es necesario ya que tenemos el mapeo)
            close(shm_fd);
            
            // Ejecutar la función del proceso hijo
            funcion_proceso(i, colores[i % num_colores], memoria, semaforo, verbose);
            
            // El proceso hijo termina aquí
            exit(EXIT_SUCCESS);
        } else {
            // Proceso padre
            pids[i] = pid; // Guardar el PID del hijo
        }
    }
    
    // ================= INICIALIZIZACIÓN COMPLETA =================
    // Ahora que todos los procesos están creados, marcamos la memoria como inicializada
    memoria->inicializado = 1;
    
    // ================= ESPERA A LOS PROCESOS HIJOS =================
    // Propósito: Sincronizar el proceso principal con los procesos secundarios
    // ¿Por qué? Si el main termina antes que los hijos, los procesos hijos pueden
    // convertirse en huérfanos y ser adoptados por el proceso init.
    for (int i = 0; i < num_procesos; i++) {
        int status;
        waitpid(pids[i], &status, 0); // Esperar a que cada proceso hijo termine
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
    printf("\n" COLOR_CYAN "=== RESUMEN DE EJECUCIÓN ===" COLOR_RESET "\n");
    printf("Contador global final: %d\n", memoria->contador_global);
    printf("Esperado: %d (%d procesos × %d iteraciones)\n\n", num_procesos * n, num_procesos, n);
    
    // ================= MEDICIONES FINALES =================
    printf(COLOR_CYAN "=== MEDICIONES FINALES ===" COLOR_RESET "\n");
    format_time_us(tiempo_fin, tiempo_buf, sizeof(tiempo_buf));
    printf("Tiempo fin: %s\n", tiempo_buf);
    
    format_bytes(memoria_fin * 1024, memoria_buf, sizeof(memoria_buf));
    printf("Memoria fin: %s\n", memoria_buf);
    printf("\n");
    
    // ================= DIFERENCIAS DE MEDICIONES =================
    // Propósito: Mostrar el consumo real de recursos durante la ejecución
    printf(COLOR_CYAN "=== DIFERENCIAS DE MEDICIONES ===" COLOR_RESET "\n");
    format_time_us(tiempo_total, tiempo_buf, sizeof(tiempo_buf));
    printf("Tiempo total: %s\n", tiempo_buf);
    
    format_bytes(memoria_usada * 1024, memoria_buf, sizeof(memoria_buf));
    printf("Memoria utilizada: %s\n", memoria_buf);
    
    // ================= MÉTRICAS DE RENDIMIENTO =================
    // Propósito: Calcular y mostrar indicadores de rendimiento del programa
    printf("\n" COLOR_CYAN "=== MÉTRICAS DE RENDIMIENTO ===" COLOR_RESET "\n");
    double operaciones = num_procesos * n;  // Total de operaciones realizadas
    double segundos = tiempo_total / 1000000.0;  // Tiempo total en segundos
    double ops_por_segundo = operaciones / segundos;  // Operaciones por segundo
    
    printf("Operaciones totales: %.0f\n", operaciones);
    printf("Operaciones por segundo: %.2f ops/s\n", ops_por_segundo);
    
    // ================= LIMPIEZA =================
    // Propósito: Liberar todos los recursos asignados antes de terminar
    
    // Cerrar el semáforo y eliminarlo
    sem_close(semaforo);
    sem_unlink(sem_name);
    
    // Desmapear la memoria compartida
    munmap(memoria, sizeof(MemoriaCompartida));
    
    // Cerrar el descriptor de archivo de memoria compartida
    close(shm_fd);
    
    // Eliminar el objeto de memoria compartida
    shm_unlink(shm_name);
    
    // Liberar memoria de los PIDs
    free(pids);
    
    return EXIT_SUCCESS; // Salir con éxito
}