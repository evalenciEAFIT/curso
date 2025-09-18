# Multiplicación de Matrices: Hilos vs Procesos

## Introducción

La multiplicación de matrices es una operación computacionalmente intensiva que se beneficia enormemente de la concurrencia. En este documento, exploraremos dos enfoques para paralelizar esta operación: usando hilos y usando procesos. Analizaremos en detalle ambas implementaciones y compararemos sus ventajas y desventajas.

## Implementación con Hilos

### Código Completo

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>
#include <pthread.h>

// Estructura para pasar datos a los hilos
typedef struct {
    int id_hilo;                // Identificador del hilo
    int fila_inicio;            // Fila inicial que procesará este hilo
    int fila_fin;               // Fila final que procesará este hilo
    int filas_A;                // Número de filas de la matriz A
    int columnas_A;             // Número de columnas de la matriz A
    int columnas_B;             // Número de columnas de la matriz B
    int **matriz_A;             // Puntero a la matriz A
    int **matriz_B;             // Puntero a la matriz B
    int **matriz_resultado;     // Puntero a la matriz resultado
    long long operaciones;      // Contador de operaciones realizadas por este hilo
} DatosHilo;

// Función que ejecutará cada hilo
void *procesarFilas(void *arg);

// Función para multiplicar matrices usando hilos
void multiplicarMatrizConcurrente(int filas_A, int columnas_A, int **matriz_A,
                                  int filas_B, int columnas_B, int **matriz_B,
                                  int **matriz_resultado, long long *operaciones,
                                  int num_hilos);

// Función para inicializar matriz con valores aleatorios
void inicializarMatriz(int filas, int columnas, int **matriz);

// Función para imprimir matriz en pantalla
void verMatriz(int filas, int columnas, int **matriz);

// Función para obtener consumo de memoria del proceso
long verConsumoMemoria();

int main(int argc, char *argv[]){    
    // Verificar argumentos de entrada
    if (argc != 6) {
        printf("Uso: %s filas_A columnas_A filas_B columnas_B num_hilos\n", argv[0]);
        return -1;
    }
    
    // Convertir argumentos a enteros
    int filas_A = atoi(argv[1]);
    int columnas_A = atoi(argv[2]);
    int filas_B = atoi(argv[3]);
    int columnas_B = atoi(argv[4]);
    int num_hilos = atoi(argv[5]);

    // Verificar compatibilidad de matrices para multiplicación
    if (columnas_A != filas_B){
        printf("Error: Columnas de A (%d) deben ser iguales a filas de B (%d)\n", 
               columnas_A, filas_B);
        return -1;
    }
    
    // Validar número de hilos
    if (num_hilos <= 0) {
        printf("Error: El número de hilos debe ser mayor que 0\n");
        return -1;
    }
    
    // Limitar número máximo de hilos para evitar sobrecarga
    if (num_hilos > 100) {
        printf("Advertencia: Limitando el número de hilos a 100\n");
        num_hilos = 100;
    }
        
    // Reservar memoria para matriz A
    int **matriz_A = (int **)malloc(filas_A * sizeof(int *));
    for(int i=0; i < filas_A; i++)
        matriz_A[i]=(int *) malloc(columnas_A * sizeof(int));

    // Reservar memoria para matriz B
    int **matriz_B = (int **)malloc(filas_B * sizeof(int *));
    for(int i=0; i < filas_B; i++)
        matriz_B[i]=(int *) malloc(columnas_B * sizeof(int));

    // Reservar memoria para matriz resultado
    int **matriz_resultado = (int **)malloc(filas_A * sizeof(int *));
    for(int i=0; i < filas_A; i++) {
        matriz_resultado[i]=(int *) malloc(columnas_B * sizeof(int));
    }
    
    // Inicializar matrices con valores aleatorios
    srand(time(NULL));
    inicializarMatriz(filas_A, columnas_A, matriz_A);
    inicializarMatriz(filas_B, columnas_B, matriz_B);

    // Medir tiempo de ejecución
    long long operaciones = 0;
    clock_t inicio, fin;
    double tiempo_CPU_usado = 0;

    inicio = clock();
    // Usar la versión concurrente de la multiplicación
    multiplicarMatrizConcurrente(filas_A, columnas_A, matriz_A,
                                 filas_B, columnas_B, matriz_B,
                                 matriz_resultado, &operaciones, num_hilos);
    fin = clock();
    tiempo_CPU_usado = ((double)(fin-inicio))/CLOCKS_PER_SEC;

    // Imprimir matrices (solo para matrices pequeñas)
    printf("\nMatriz A: \n");
    verMatriz(filas_A, columnas_A, matriz_A);
    printf("\nMatriz B: \n");
    verMatriz(filas_B, columnas_B, matriz_B);
    printf("\nMatriz Resultado: \n");
    verMatriz(filas_A, columnas_B, matriz_resultado);

    // Mostrar estadísticas de rendimiento
    printf("\nESTADISTICAS...\n");
    printf("Dimensiones: A[%d][%d] B[%d][%d]\n",
           filas_A, columnas_A, filas_B, columnas_B);
    printf("Tiempo de CPU: %.4f segundos\n", tiempo_CPU_usado);
    printf("Consumo de memoria: %ld KB\n", verConsumoMemoria());
    printf("Total operaciones: %lld op.\n", operaciones);
    printf("Número de hilos utilizados: %d\n", num_hilos);
 
    // Liberar memoria de matriz A
    for(int i=0; i<filas_A; i++) free(matriz_A[i]);
    free(matriz_A);

    // Liberar memoria de matriz B
    for(int i=0; i<filas_B; i++) free(matriz_B[i]);
    free(matriz_B);

    // Liberar memoria de matriz resultado
    for(int i=0; i<filas_A; i++) free(matriz_resultado[i]);
    free(matriz_resultado);
    
    return 0;
}

// Función que ejecutará cada hilo para procesar un conjunto de filas
void *procesarFilas(void *arg) {
    // Convertir el argumento a la estructura de datos
    DatosHilo *datos = (DatosHilo *)arg;
    
    // Inicializar el contador de operaciones para este hilo
    datos->operaciones = 0;
    
    // Procesar las filas asignadas a este hilo
    for(int i=datos->fila_inicio; i<datos->fila_fin; i++){
        for(int j=0; j<datos->columnas_B; j++){
            // Inicializar el elemento de la matriz resultado
            datos->matriz_resultado[i][j] = 0;
            
            // Calcular el producto punto de la fila i de A y la columna j de B
            for(int k=0; k<datos->columnas_A; k++){
                datos->matriz_resultado[i][j] += datos->matriz_A[i][k] * datos->matriz_B[k][j];
                // Incrementar el contador de operaciones (una multiplicación y una suma)
                datos->operaciones += 2;
            }
        }
    }
    
    // Finalizar el hilo
    pthread_exit(NULL);
}

// Función principal de multiplicación concurrente
void multiplicarMatrizConcurrente(int filas_A, int columnas_A, int **matriz_A,
                                  int filas_B, int columnas_B, int **matriz_B,
                                  int **matriz_resultado, long long *operaciones,
                                  int num_hilos) {
    
    // Arreglo para almacenar los identificadores de los hilos
    pthread_t *hilos = (pthread_t *)malloc(num_hilos * sizeof(pthread_t));
    
    // Arreglo para almacenar los datos de cada hilo
    DatosHilo *datos_hilos = (DatosHilo *)malloc(num_hilos * sizeof(DatosHilo));
    
    // Calcular cuántas filas procesará cada hilo
    int filas_por_hilo = filas_A / num_hilos;
    
    // Inicializar el contador total de operaciones
    *operaciones = 0;
    
    // Crear los hilos
    for(int i=0; i<num_hilos; i++) {
        // Configurar los datos para este hilo
        datos_hilos[i].id_hilo = i;
        datos_hilos[i].fila_inicio = i * filas_por_hilo;
        datos_hilos[i].fila_fin = (i+1) * filas_por_hilo;
        
        // Si es el último hilo, asignarle las filas restantes
        if(i == num_hilos-1) {
            datos_hilos[i].fila_fin = filas_A;
        }
        
        // Pasar las dimensiones de las matrices
        datos_hilos[i].filas_A = filas_A;
        datos_hilos[i].columnas_A = columnas_A;
        datos_hilos[i].columnas_B = columnas_B;
        
        // Pasar los punteros a las matrices
        datos_hilos[i].matriz_A = matriz_A;
        datos_hilos[i].matriz_B = matriz_B;
        datos_hilos[i].matriz_resultado = matriz_resultado;
        
        // Crear el hilo
        pthread_create(&hilos[i], NULL, procesarFilas, &datos_hilos[i]);
    }
    
    // Esperar a que todos los hilos terminen
    for(int i=0; i<num_hilos; i++) {
        pthread_join(hilos[i], NULL);
        // Sumar las operaciones realizadas por este hilo al total
        *operaciones += datos_hilos[i].operaciones;
    }
    
    // Liberar memoria reservada para los hilos y datos
    free(hilos);
    free(datos_hilos);
}

// Inicializa matriz con valores aleatorios entre 0 y 9
void inicializarMatriz(int filas, int columnas, int **matriz){
   for(int i=0; i<filas; i++){
    for(int j=0; j<columnas; j++){
        matriz[i][j] = rand() % 10;
    }
   }
}

// Imprime contenido de matriz en pantalla
void verMatriz(int filas, int columnas, int **matriz){
    for(int i=0; i<filas; i++){
        for(int j=0; j<columnas; j++){
            printf("%d\t", matriz[i][j]);
        }
        printf("\n");
   }
}

// Obtiene consumo de memoria desde /proc/self/status
long verConsumoMemoria(){
   FILE* archivo = fopen("/proc/self/status","r");
   if (!archivo) {
       perror("Error al abrir archivo de estado");
       return -1;
   }
   
   char linea[128];
   long uso_memoria = 0;
   
   // Buscar línea con VmRSS (memoria física usada)
   while(fgets(linea, 128, archivo) != NULL){
      if (strncmp(linea, "VmRSS:", 6) == 0){
        sscanf(linea + 6, "%ld", &uso_memoria);
        break;
      }
   }
   
   fclose(archivo);
   return uso_memoria;
}
```

### Explicación Detallada

#### Estructura de Datos para Hilos (DatosHilo)

La estructura `DatosHilo` encapsula toda la información que un hilo necesita para realizar su trabajo:

- **id_hilo**: Identificador único para cada hilo.
- **fila_inicio y fila_fin**: Definen el rango de filas de la matriz resultado que este hilo debe calcular.
- **Dimensiones de matrices**: Necesarias para los bucles de cálculo.
- **Punteros a matrices**: Todos los hilos comparten las mismas matrices, pero cada hilo escribe en una región distinta de la matriz resultado.
- **operaciones**: Contador local de operaciones realizadas por este hilo.

#### Función del Hilo (procesarFilas)

Esta función es ejecutada por cada hilo:

1. **Recepción de parámetros**: Convierte el puntero void* a la estructura DatosHilo.
2. **Inicialización del contador local**: Cada hilo tiene su propio contador para evitar condiciones de carrera.
3. **Procesamiento de filas asignadas**: El hilo procesa solo las filas desde `fila_inicio` hasta `fila_fin-1`.
4. **Cálculo del producto punto**: Para cada elemento de la matriz resultado, calcula el producto punto de la fila correspondiente de A y la columna correspondiente de B.
5. **Finalización del hilo**: Termina con `pthread_exit(NULL)`.

#### Función de Multiplicación Concurrente (multiplicarMatrizConcurrente)

Esta función coordina la creación y sincronización de los hilos:

1. **Reserva de memoria**: Para los identificadores de hilos y las estructuras de datos.
2. **Cálculo de distribución de trabajo**: Divide las filas entre los hilos de manera equitativa.
3. **Creación de hilos**: Crea cada hilo con `pthread_create`, pasándole los datos necesarios.
4. **Sincronización**: Espera a que todos los hilos terminen con `pthread_join`.
5. **Recopilación de resultados**: Suma las operaciones realizadas por cada hilo.

## Implementación con Procesos

### Código Completo

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

// Estructura para pasar datos a los procesos
typedef struct {
    int id_proceso;             // Identificador del proceso
    int fila_inicio;            // Fila inicial que procesará este proceso
    int fila_fin;               // Fila final que procesará este proceso
    int filas_A;                // Número de filas de la matriz A
    int columnas_A;             // Número de columnas de la matriz A
    int columnas_B;             // Número de columnas de la matriz B
    int **matriz_A;             // Puntero a la matriz A
    int **matriz_B;             // Puntero a la matriz B
    int **matriz_resultado;     // Puntero a la matriz resultado
    long long operaciones;      // Contador de operaciones realizadas por este proceso
} DatosProceso;

// Función para multiplicar matrices usando procesos
void multiplicarMatrizProcesos(int filas_A, int columnas_A, int **matriz_A,
                               int filas_B, int columnas_B, int **matriz_B,
                               int **matriz_resultado, long long *operaciones,
                               int num_procesos);

// Función para inicializar matriz con valores aleatorios
void inicializarMatriz(int filas, int columnas, int **matriz);

// Función para imprimir matriz en pantalla
void verMatriz(int filas, int columnas, int **matriz);

// Función para obtener consumo de memoria del proceso
long verConsumoMemoria();

// Función que ejecutará cada proceso
void procesarFilasProceso(DatosProceso datos);

int main(int argc, char *argv[]){    
    // Verificar argumentos de entrada
    if (argc != 6) {
        printf("Uso: %s filas_A columnas_A filas_B columnas_B num_procesos\n", argv[0]);
        return -1;
    }
    
    // Convertir argumentos a enteros
    int filas_A = atoi(argv[1]);
    int columnas_A = atoi(argv[2]);
    int filas_B = atoi(argv[3]);
    int columnas_B = atoi(argv[4]);
    int num_procesos = atoi(argv[5]);

    // Verificar compatibilidad de matrices para multiplicación
    if (columnas_A != filas_B){
        printf("Error: Columnas de A (%d) deben ser iguales a filas de B (%d)\n", 
               columnas_A, filas_B);
        return -1;
    }
    
    // Validar número de procesos
    if (num_procesos <= 0) {
        printf("Error: El número de procesos debe ser mayor que 0\n");
        return -1;
    }
    
    // Limitar número máximo de procesos para evitar sobrecarga
    if (num_procesos > 20) {
        printf("Advertencia: Limitando el número de procesos a 20\n");
        num_procesos = 20;
    }
        
    // Reservar memoria para matriz A
    int **matriz_A = (int **)malloc(filas_A * sizeof(int *));
    for(int i=0; i < filas_A; i++)
        matriz_A[i]=(int *) malloc(columnas_A * sizeof(int));

    // Reservar memoria para matriz B
    int **matriz_B = (int **)malloc(filas_B * sizeof(int *));
    for(int i=0; i < filas_B; i++)
        matriz_B[i]=(int *) malloc(columnas_B * sizeof(int));

    // Reservar memoria para matriz resultado
    int **matriz_resultado = (int **)malloc(filas_A * sizeof(int *));
    for(int i=0; i < filas_A; i++) {
        matriz_resultado[i]=(int *) malloc(columnas_B * sizeof(int));
    }
    
    // Inicializar matrices con valores aleatorios
    srand(time(NULL));
    inicializarMatriz(filas_A, columnas_A, matriz_A);
    inicializarMatriz(filas_B, columnas_B, matriz_B);

    // Medir tiempo de ejecución
    long long operaciones = 0;
    clock_t inicio, fin;
    double tiempo_CPU_usado = 0;

    inicio = clock();
    // Usar la versión con procesos de la multiplicación
    multiplicarMatrizProcesos(filas_A, columnas_A, matriz_A,
                              filas_B, columnas_B, matriz_B,
                              matriz_resultado, &operaciones, num_procesos);
    fin = clock();
    tiempo_CPU_usado = ((double)(fin-inicio))/CLOCKS_PER_SEC;

    // Imprimir matrices (solo para matrices pequeñas)
    printf("\nMatriz A: \n");
    verMatriz(filas_A, columnas_A, matriz_A);
    printf("\nMatriz B: \n");
    verMatriz(filas_B, columnas_B, matriz_B);
    printf("\nMatriz Resultado: \n");
    verMatriz(filas_A, columnas_B, matriz_resultado);

    // Mostrar estadísticas de rendimiento
    printf("\nESTADISTICAS...\n");
    printf("Dimensiones: A[%d][%d] B[%d][%d]\n",
           filas_A, columnas_A, filas_B, columnas_B);
    printf("Tiempo de CPU: %.4f segundos\n", tiempo_CPU_usado);
    printf("Consumo de memoria: %ld KB\n", verConsumoMemoria());
    printf("Total operaciones: %lld op.\n", operaciones);
    printf("Número de procesos utilizados: %d\n", num_procesos);
 
    // Liberar memoria de matriz A
    for(int i=0; i<filas_A; i++) free(matriz_A[i]);
    free(matriz_A);

    // Liberar memoria de matriz B
    for(int i=0; i<filas_B; i++) free(matriz_B[i]);
    free(matriz_B);

    // Liberar memoria de matriz resultado
    for(int i=0; i<filas_A; i++) free(matriz_resultado[i]);
    free(matriz_resultado);
    
    return 0;
}

// Función que ejecutará cada proceso para procesar un conjunto de filas
void procesarFilasProceso(DatosProceso datos) {
    // Inicializar el contador de operaciones para este proceso
    datos.operaciones = 0;
    
    // Procesar las filas asignadas a este proceso
    for(int i=datos.fila_inicio; i<datos.fila_fin; i++){
        for(int j=0; j<datos.columnas_B; j++){
            // Inicializar el elemento de la matriz resultado
            datos.matriz_resultado[i][j] = 0;
            
            // Calcular el producto punto de la fila i de A y la columna j de B
            for(int k=0; k<datos.columnas_A; k++){
                datos.matriz_resultado[i][j] += datos.matriz_A[i][k] * datos.matriz_B[k][j];
                // Incrementar el contador de operaciones (una multiplicación y una suma)
                datos.operaciones += 2;
            }
        }
    }
    
    // Salir del proceso con el número de operaciones como código de salida
    exit(datos.operaciones);
}

// Función principal de multiplicación con procesos
void multiplicarMatrizProcesos(int filas_A, int columnas_A, int **matriz_A,
                               int filas_B, int columnas_B, int **matriz_B,
                               int **matriz_resultado, long long *operaciones,
                               int num_procesos) {
    
    // Arreglo para almacenar los identificadores de los procesos
    pid_t *procesos = (pid_t *)malloc(num_procesos * sizeof(pid_t));
    
    // Arreglo para almacenar los datos de cada proceso
    DatosProceso *datos_procesos = (DatosProceso *)malloc(num_procesos * sizeof(DatosProceso));
    
    // Calcular cuántas filas procesará cada proceso
    int filas_por_proceso = filas_A / num_procesos;
    
    // Inicializar el contador total de operaciones
    *operaciones = 0;
    
    // Crear los procesos
    for(int i=0; i<num_procesos; i++) {
        // Configurar los datos para este proceso
        datos_procesos[i].id_proceso = i;
        datos_procesos[i].fila_inicio = i * filas_por_proceso;
        datos_procesos[i].fila_fin = (i+1) * filas_por_proceso;
        
        // Si es el último proceso, asignarle las filas restantes
        if(i == num_procesos-1) {
            datos_procesos[i].fila_fin = filas_A;
        }
        
        // Pasar las dimensiones de las matrices
        datos_procesos[i].filas_A = filas_A;
        datos_procesos[i].columnas_A = columnas_A;
        datos_procesos[i].columnas_B = columnas_B;
        
        // Pasar los punteros a las matrices
        datos_procesos[i].matriz_A = matriz_A;
        datos_procesos[i].matriz_B = matriz_B;
        datos_procesos[i].matriz_resultado = matriz_resultado;
        
        // Crear el proceso
        procesos[i] = fork();
        
        if (procesos[i] == 0) {
            // Proceso hijo
            procesarFilasProceso(datos_procesos[i]);
        } else if (procesos[i] < 0) {
            // Error al crear el proceso
            perror("Error al crear proceso");
            exit(1);
        }
        // Proceso padre continúa con el siguiente proceso
    }
    
    // Esperar a que todos los procesos terminen
    int estado;
    for(int i=0; i<num_procesos; i++) {
        waitpid(procesos[i], &estado, 0);
        // Obtener el número de operaciones del código de salida
        if (WIFEXITED(estado)) {
            *operaciones += WEXITSTATUS(estado);
        }
    }
    
    // Liberar memoria reservada para los procesos y datos
    free(procesos);
    free(datos_procesos);
}

// Inicializa matriz con valores aleatorios entre 0 y 9
void inicializarMatriz(int filas, int columnas, int **matriz){
   for(int i=0; i<filas; i++){
    for(int j=0; j<columnas; j++){
        matriz[i][j] = rand() % 10;
    }
   }
}

// Imprime contenido de matriz en pantalla
void verMatriz(int filas, int columnas, int **matriz){
    for(int i=0; i<filas; i++){
        for(int j=0; j<columnas; j++){
            printf("%d\t", matriz[i][j]);
        }
        printf("\n");
   }
}

// Obtiene consumo de memoria desde /proc/self/status
long verConsumoMemoria(){
   FILE* archivo = fopen("/proc/self/status","r");
   if (!archivo) {
       perror("Error al abrir archivo de estado");
       return -1;
   }
   
   char linea[128];
   long uso_memoria = 0;
   
   // Buscar línea con VmRSS (memoria física usada)
   while(fgets(linea, 128, archivo) != NULL){
      if (strncmp(linea, "VmRSS:", 6) == 0){
        sscanf(linea + 6, "%ld", &uso_memoria);
        break;
      }
   }
   
   fclose(archivo);
   return uso_memoria;
}
```

### Explicación Detallada

#### Estructura de Datos para Procesos (DatosProceso)

Similar a la estructura para hilos, `DatosProceso` encapsula la información que cada proceso necesita:

- **id_proceso**: Identificador único para cada proceso.
- **fila_inicio y fila_fin**: Definen el rango de filas que este proceso debe calcular.
- **Dimensiones de matrices y punteros**: Igual que en la versión con hilos.

#### Función del Proceso (procesarFilasProceso)

Esta función es ejecutada por cada proceso hijo:

1. **Inicialización del contador local**: Cada proceso tiene su propio contador.
2. **Procesamiento de filas asignadas**: El proceso procesa solo las filas desde `fila_inicio` hasta `fila_fin-1`.
3. **Cálculo del producto punto**: Igual que en la versión con hilos.
4. **Finalización del proceso**: Termina con `exit(datos.operaciones)`, pasando el número de operaciones como código de salida.

#### Función de Multiplicación con Procesos (multiplicarMatrizProcesos)

Esta función coordina la creación y sincronización de los procesos:

1. **Reserva de memoria**: Para los identificadores de procesos y las estructuras de datos.
2. **Cálculo de distribución de trabajo**: Divide las filas entre los procesos.
3. **Creación de procesos**: 
   - Usa `fork()` para crear cada proceso hijo.
   - El proceso hijo ejecuta `procesarFilasProceso`.
   - El proceso padre continúa creando más procesos.
4. **Sincronización**: 
   - Usa `waitpid()` para esperar a que cada proceso hijo termine.
   - Obtiene el número de operaciones del código de salida del proceso.

## Comparación: Hilos vs Procesos

### Conceptos Fundamentales

#### Hilos
- **Definición**: Unidades básicas de ejecución dentro de un proceso.
- **Recursos compartidos**: Todos los hilos de un proceso comparten el mismo espacio de direcciones (memoria, archivos, etc.).
- **Contexto propio**: Cada hilo tiene su propia pila, registros y estado de ejecución.
- **Creación y gestión**: Más ligeros que los procesos, menor overhead.

#### Procesos
- **Definición**: Unidades de ejecución independientes con su propio espacio de direcciones.
- **Recursos aislados**: Cada proceso tiene su propio espacio de memoria, archivos abiertos, etc.
- **Comunicación**: Requiere mecanismos explícitos (pipes, memoria compartida, sockets).
- **Creación y gestión**: Más pesados que los hilos, mayor overhead.

### Ventajas y Desventajas

#### Hilos

**Ventajas:**
1. **Menor overhead de creación**: Crear un hilo es más rápido que crear un proceso.
2. **Compartición de memoria**: Los hilos comparten automáticamente el espacio de direcciones, facilitando el intercambio de datos.
3. **Comunicación eficiente**: La comunicación entre hilos es tan simple como acceder a variables compartidas.
4. **Menor consumo de recursos**: Los hilos consumen menos recursos del sistema que los procesos.
5. **Cambio de contexto más rápido**: El cambio de contexto entre hilos es más rápido que entre procesos.

**Desventajas:**
1. **Menor aislamiento**: Un error en un hilo puede afectar a todo el proceso.
2. **Problemas de concurrencia**: Se requiere sincronización explícita (mutexes, semáforos) para evitar condiciones de carrera.
3. **Menor robustez**: Si un hilo falla, todo el proceso puede terminar.
4. **Dificultad de depuración**: Depurar programas con múltiples hilos es más complejo.

#### Procesos

**Ventajas:**
1. **Mayor aislamiento**: Cada proceso tiene su propio espacio de direcciones, un error en un proceso no afecta a otros.
2. **Mayor robustez**: Si un proceso falla, los demás pueden continuar ejecutándose.
3. **Mayor seguridad**: El aislamiento de memoria proporciona una capa adicional de seguridad.
4. **Mejor aprovechamiento de sistemas multiprocesador**: Los procesos pueden ejecutarse en diferentes procesadores sin interferencia.
5. **Distribución en múltiples máquinas**: Los procesos pueden distribuirse en diferentes máquinas de una red.

**Desventajas:**
1. **Mayor overhead de creación**: Crear un proceso es más costoso en tiempo y recursos.
2. **Comunicación compleja**: Requiere mecanismos explícitos de comunicación entre procesos (IPC).
3. **Mayor consumo de recursos**: Cada proceso consume más recursos del sistema.
4. **Cambio de contexto más lento**: El cambio de contexto entre procesos es más costoso.
5. **Dificultad para compartir datos**: Compartir datos entre procesos requiere configuración explícita (memoria compartida).

### Análisis Comparativo para la Multiplicación de Matrices

#### Comunicación de Datos
- **Hilos**: Los hilos comparten automáticamente las matrices, no se requiere mecanismo adicional.
- **Procesos**: En nuestra implementación, los procesos heredan las matrices del proceso padre después de `fork()`, lo que permite compartir los datos sin configuración adicional. Sin embargo, en general, la comunicación entre procesos requiere mecanismos explícitos.

#### Sincronización
- **Hilos**: Se usa `pthread_join` para esperar a que todos los hilos terminen.
- **Procesos**: Se usa `waitpid` para esperar a que todos los procesos terminen.

#### Recopilación de Resultados
- **Hilos**: Cada hilo actualiza su contador local, y el hilo principal suma todos los contadores.
- **Procesos**: Cada proceso devuelve su contador como código de salida, y el proceso principal suma todos los contadores.

#### Rendimiento
- **Creación**: Los hilos son más rápidos de crear que los procesos.
- **Comunicación**: Los hilos tienen ventaja al compartir memoria automáticamente.
- **Escalabilidad**: Para sistemas con múltiples núcleos, ambos enfoques pueden aprovechar el paralelismo, pero los procesos pueden tener una ligera ventaja en sistemas con muchos núcleos debido al mejor aislamiento.

#### Uso de Recursos
- **Memoria**: Los hilos consumen menos memoria ya que comparten el espacio de direcciones.
- **CPU**: El cambio de contexto entre hilos es más eficiente.

### Cuadro Resumen

| Aspecto                | Hilos                                  | Procesos                                |
|-----------------------|----------------------------------------|-----------------------------------------|
| Creación              | Rápida, bajo overhead                  | Lenta, alto overhead                    |
| Compartición de datos | Automática, espacio compartido         | Requiere mecanismos explícitos (IPC)    |
| Comunicación          | Simple (variables compartidas)         | Compleja (pipes, memoria compartida)    |
| Aislamiento           | Bajo (un error afecta a todo el proceso)| Alto (procesos independientes)          |
| Robustez              | Menor (un hilo puede afectar a todos)  | Mayor (fallos aislados)                 |
| Consumo de recursos   | Menor                                  | Mayor                                   |
| Cambio de contexto    | Rápido                                 | Lento                                   |
| Depuración            | Más compleja                           | Más sencilla                            |
| Seguridad             | Menor (acceso compartido)              | Mayor (aislamiento)                     |

## Conclusiones

La elección entre hilos y procesos para la multiplicación de matrices depende de varios factores:

### Cuándo Usar Hilos
1. Cuando se necesita máxima eficiencia y rendimiento.
2. Cuando la comunicación entre unidades de ejecución es intensiva.
3. Cuando se trabaja en un entorno controlado y seguro.
4. Cuando el overhead de creación y gestión es crítico.
5. Cuando se está desarrollando para un sistema con recursos limitados.

### Cuándo Usar Procesos
1. Cuando se requiere mayor robustez y aislamiento.
2. Cuando las unidades de ejecución son independientes y no necesitan comunicarse frecuentemente.
3. Cuando se trabaja en un entorno donde la seguridad es crítica.
4. Cuando se necesita distribuir la carga en múltiples máquinas.
5. Cuando la fiabilidad es más importante que el rendimiento máximo.

### Recomendaciones para la Multiplicación de Matrices
Para la multiplicación de matrices específicamente:

- **Hilos** son generalmente más eficientes debido a:
  - La necesidad de compartir grandes cantidades de datos (las matrices).
  - La comunicación intensiva entre las unidades de ejecución.
  - El menor overhead de creación y gestión.

- **Procesos** pueden ser preferibles cuando:
  - Se necesita mayor robustez y aislamiento.
  - Se está procesando matrices extremadamente grandes y se quiere evitar que un error en una parte afecte a todo el cálculo.
  - Se está distribuyendo el cálculo en múltiples máquinas.

En la práctica, para la mayoría de los casos de multiplicación de matrices en un solo sistema, los hilos ofrecen un mejor equilibrio entre rendimiento y complejidad de implementación. Sin embargo, la elección final debe basarse en los requisitos específicos de cada aplicación.
