#include <stdio.h>          // Para entrada/salida estándar (printf, fprintf, etc.)
#include <stdlib.h>         // Para funciones estándar (atoi, malloc, free, exit)
#include <pthread.h>        // Para manejo de hilos POSIX
#include <unistd.h>         // Para funciones POSIX (usleep)
#include <sys/time.h>       // Para medición de tiempo (gettimeofday)
#include <sys/resource.h>   // Para obtener uso de recursos (getrusage)
#include <time.h>           // Para funciones de tiempo (time)
#include <string.h>         // Para manipulación de cadenas (strcmp)
#include <getopt.h>         // Para parsear opciones de línea de comandos

// ================= DEFINICIÓN DE COLORES =================
// Códigos ANSI para colores en terminal. Permiten diferenciar visualmente
// la salida de cada hilo, facilitando la comprensión del flujo de ejecución.
#define COLOR_ROJO     "\033[1;31m"      // Rojo brillante
#define COLOR_VERDE    "\033[1;32m"      // Verde brillante
#define COLOR_AZUL     "\033[1;34m"      // Azul brillante
#define COLOR_AMARILLO "\033[1;33m"      // Amarillo brillante
#define COLOR_MAGENTA  "\033[1;35m"      // Magenta brillante
#define COLOR_CYAN     "\033[1;36m"      // Cian brillante
#define COLOR_BLANCO   "\033[1;37m"      // Blanco brillante
#define COLOR_RESET    "\033[0m"         // Resetear color a valor por defecto

// ================= MEMORIA COMPARTIDA =================
// Estructura que contiene los datos compartidos entre todos los hilos.
// En programación concurrente, la memoria compartida permite que múltiples
// hilos accedan a los mismos datos, pero requiere mecanismos de sincronización
// para evitar condiciones de carrera.
typedef struct {
    int contador_global;     // Variable compartida que todos los hilos modifican
    pthread_mutex_t mutex;   // Mutex para proteger el acceso concurrente
    int iteraciones;         // Número de iteraciones para cada hilo
} MemoriaCompartida;

// ================= DATOS DEL HILO =================
// Estructura que contiene información específica para cada hilo.
// Cada hilo necesita sus propios datos locales además de acceder a la memoria compartida.
typedef struct {
    int id_hilo;             // Identificador único del hilo
    const char* color;       // Color ANSI para la salida en terminal
    MemoriaCompartida* memoria; // Puntero a la memoria compartida
    int verbose;             // Bandera para mostrar iteraciones (1=mostrar, 0=no mostrar)
} DatosHilo;

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

// ================= FUNCIÓN DEL HILO =================
// Función que ejecutará cada hilo. Demuestra el uso de memoria compartida
// y sincronización en un entorno multihilo.
// Propósito: Realizar tareas concurrentes con acceso controlado a recursos compartidos
// Cómo funciona: Cada hilo ejecuta un bucle de iteraciones, simulando trabajo
//               y accediendo a memoria compartida de forma segura.
void* funcion_hilo(void* arg) {
    DatosHilo* datos = (DatosHilo*)arg; // Convertimos el argumento genérico a DatosHilo
    int contador_local = 0;    // Contador local (no compartido) para cada hilo
    
    // ================= GENERADOR ALEATORIO =================
    // Cada hilo necesita su propio generador de números aleatorios con una
    // semilla única para evitar que todos generen la misma secuencia.
    // Propósito: Simular cargas de trabajo variables
    unsigned int seed = time(NULL) + datos->id_hilo; // Semilla única (tiempo + ID del hilo)
    srand(seed);             // Inicializamos el generador con la semilla
    
    // Si está en modo verbose, mostramos mensaje de inicio
    if (datos->verbose) {
        printf("%sHilo %d: Iniciando con %d iteraciones%s\n", 
               datos->color, datos->id_hilo, datos->memoria->iteraciones, COLOR_RESET);
    }
    
    // ================= BUCLE DE TRABAJO =================
    // Cada hilo realiza 'n' iteraciones donde simula trabajo y accede
    // a recursos compartidos de forma segura.
    // Propósito: Simular un procesamiento concurrente con carga variable
    for (int i = 0; i < datos->memoria->iteraciones; i++) {
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
        // múltiples hilos podrían interferir entre sí si se ejecutan
        // simultáneamente. Debe protegerse con mecanismos de sincronización.
        // Propósito: Garantizar la integridad de los datos compartidos
        pthread_mutex_lock(&datos->memoria->mutex); // Bloqueamos el mutex
        
        // ================= MODIFICACIÓN DE DATOS COMPARTIDOS =================
        // Ahora que tenemos acceso exclusivo, podemos modificar el contador
        // global de forma segura.
        datos->memoria->contador_global++; // Incrementamos contador compartido
        contador_local++;                 // Incrementamos contador local (no necesita protección)
        
        // ================= SALIDA FORMATEADA =================
        // Mostramos información con el color del hilo para facilitar
        // la identificación visual de qué hilo está ejecutándose.
        // Solo mostramos si está en modo verbose
        if (datos->verbose) {
            printf("%sHilo %d: Iteración %d/%d | Contador global: %d | Contador local: %d%s\n",
                   datos->color, datos->id_hilo, i+1, datos->memoria->iteraciones, 
                   datos->memoria->contador_global, contador_local, COLOR_RESET);
        }
        
        pthread_mutex_unlock(&datos->memoria->mutex); // Desbloqueamos el mutex
    }
    
    // Si está en modo verbose, mostramos mensaje de finalización
    if (datos->verbose) {
        printf("%sHilo %d: Finalizado. Realizó %d iteraciones%s\n", 
               datos->color, datos->id_hilo, contador_local, COLOR_RESET);
    }
    
    // Liberamos la memoria asignada para los datos del hilo
    free(datos);
    return NULL;
}

// ================= FUNCIÓN DE AYUDA =================
// Propósito: Mostrar información de uso del programa
// Cómo funciona: Imprime un mensaje formateado con las opciones disponibles
void print_usage(const char* program_name) {
    printf("Uso: %s [OPCIONES]\n", program_name);
    printf("Opciones:\n");
    printf("  -h, --hilos NUM_H     Número de hilos a crear (obligatorio)\n");
    printf("  -i, --iteraciones NUM_I Número de iteraciones por hilo (obligatorio)\n");
    printf("  -v, --verbose         Mostrar información detallada de las iteraciones\n");
    printf("  --help               Mostrar este mensaje de ayuda\n");
    printf("\nEjemplos:\n");
    printf("  %s --hilos 4 --iteraciones 5\n", program_name);
    printf("  %s -h 6 -i 10 -v\n", program_name);
}

// ================= FUNCIÓN PRINCIPAL =================
// Propósito: Punto de entrada del programa. Configura y ejecuta los hilos,
//            procesa argumentos de línea de comandos y muestra resultados.
int main(int argc, char* argv[]) {
    int num_hilos = 0;        // Número de hilos a crear
    int n = 0;                // Número de iteraciones por hilo
    int verbose = 0;          // Bandera para modo verbose (0=desactivado, 1=activado)
    
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
                num_hilos = atoi(optarg); // Convertir argumento a entero
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
    if (num_hilos <= 0 || n <= 0) {
        fprintf(stderr, "Error: Debes especificar el número de hilos e iteraciones\n\n");
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }
    
    // Limitar el número máximo de hilos para evitar problemas de rendimiento
    if (num_hilos > 20) {
        fprintf(stderr, "Error: El número máximo de hilos es 20\n");
        return EXIT_FAILURE;
    }
    
    // ================= MEDICIONES INICIALES =================
    // Propósito: Establecer una línea base para medir el rendimiento
    long long tiempo_inicio = get_time_us();       // Tiempo inicial en microsegundos
    long memoria_inicio = get_memory_usage();      // Memoria inicial en KB
    
    // Buffers para formatear las mediciones
    char tiempo_buf[32], memoria_buf[32];
    
    // Mostrar mediciones iniciales
    printf(COLOR_CYAN "=== MEDICIONES INICIALES (c) ===" COLOR_RESET "\n");
    format_time_us(tiempo_inicio, tiempo_buf, sizeof(tiempo_buf));
    printf("Tiempo inicio: %s\n", tiempo_buf);
    
    format_bytes(memoria_inicio * 1024, memoria_buf, sizeof(memoria_buf));
    printf("Memoria inicio: %s\n", memoria_buf);
    printf("\n");
    
    // ================= INICIALIZACIÓN =================
    // Propósito: Preparar las estructuras de datos y recursos necesarios
    MemoriaCompartida memoria;
    memoria.contador_global = 0; // Inicializar contador global en 0
    memoria.iteraciones = n;      // Establecer número de iteraciones
    
    // Inicializar el mutex para proteger el acceso a memoria compartida
    if (pthread_mutex_init(&memoria.mutex, NULL) != 0) {
        perror("Error al inicializar el mutex"); // Mostrar error del sistema
        return EXIT_FAILURE;
    }
    
    // Asignar memoria dinámica para el array de hilos
    pthread_t* hilos = malloc(sizeof(pthread_t) * num_hilos);
    if (hilos == NULL) {
        perror("Error al asignar memoria para los hilos");
        return EXIT_FAILURE;
    }
    
    // Array de colores disponibles para los hilos
    const char* colores[] = {
        COLOR_ROJO, COLOR_VERDE, COLOR_AZUL, COLOR_AMARILLO, 
        COLOR_MAGENTA, COLOR_CYAN, COLOR_BLANCO
    };
    int num_colores = sizeof(colores) / sizeof(colores[0]); // Número de colores disponibles
    
    // Mostrar información de configuración
    printf("Creando %d hilos con %d iteraciones cada uno...\n", num_hilos, n);
    if (!verbose) {
        printf("Ejecutando en modo silencioso. Usa --verbose para ver detalles.\n\n");
    } else {
        printf("\n");
    }
    
    // ================= CREACIÓN DE HILOS =================
    // Propósito: Crear e iniciar los hilos que ejecutarán las tareas concurrentes
    for (int i = 0; i < num_hilos; i++) {
        // Asignar memoria para los datos específicos del hilo
        DatosHilo* datos = malloc(sizeof(DatosHilo));
        if (datos == NULL) {
            perror("Error al asignar memoria para datos del hilo");
            free(hilos); // Liberar memoria previamente asignada
            return EXIT_FAILURE;
        }
        
        // Configurar los datos del hilo
        datos->id_hilo = i;                    // Asignar ID único
        datos->color = colores[i % num_colores]; // Asignar color cíclicamente
        datos->memoria = &memoria;              // Puntero a memoria compartida
        datos->verbose = verbose;               // Pasar bandera verbose
        
        // Crear el hilo con la función funcion_hilo y los datos
        if (pthread_create(&hilos[i], NULL, funcion_hilo, datos) != 0) {
            perror("Error al crear el hilo");
            free(datos); // Liberar datos del hilo actual
            free(hilos); // Liberar array de hilos
            return EXIT_FAILURE;
        }
    }
    
    // ================= ESPERA A LOS HILOS =================
    // Propósito: Sincronizar el hilo principal con los hilos secundarios
    // ¿Por qué? Si el main termina antes que los hilos, el programa se cerrará
    // abruptamente, terminando todos los hilos antes de que completen su trabajo.
    for (int i = 0; i < num_hilos; i++) {
        pthread_join(hilos[i], NULL); // Esperar a que cada hilo termine
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
    printf("Contador global final: %d\n", memoria.contador_global);
    printf("Esperado: %d (%d hilos × %d iteraciones)\n\n", num_hilos * n, num_hilos, n);
    
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
    double operaciones = num_hilos * n;  // Total de operaciones realizadas
    double segundos = tiempo_total / 1000000.0;  // Tiempo total en segundos
    double ops_por_segundo = operaciones / segundos;  // Operaciones por segundo
    
    printf("Operaciones totales: %.0f\n", operaciones);
    printf("Operaciones por segundo: %.2f ops/s\n", ops_por_segundo);
    
    // ================= LIMPIEZA =================
    // Propósito: Liberar todos los recursos asignados antes de terminar
    pthread_mutex_destroy(&memoria.mutex); // Destruir el mutex
    free(hilos); // Liberar array de hilos
    
    return EXIT_SUCCESS; // Salir con éxito
}
