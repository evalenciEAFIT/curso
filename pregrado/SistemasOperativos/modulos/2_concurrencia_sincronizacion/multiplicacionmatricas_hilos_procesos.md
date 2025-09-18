# Multiplicación de Matrices: Hilos vs Procesos
[Regresar](readme.md)   
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
    int *matriz_A;              // Puntero a la matriz A (en memoria compartida)
    int *matriz_B;              // Puntero a la matriz B (en memoria compartida)
    int *matriz_resultado;      // Puntero a la matriz resultado (en memoria compartida)
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

// Función para convertir matriz 2D a 1D para memoria compartida
int *matriz2Da1D(int **matriz, int filas, int columnas);

// Función para convertir matriz 1D a 2D desde memoria compartida
int **matriz1Da2D(int *matriz1D, int filas, int columnas);

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

// Función para convertir matriz 2D a 1D para memoria compartida
int *matriz2Da1D(int **matriz, int filas, int columnas) {
    int *matriz1D = (int *)malloc(filas * columnas * sizeof(int));
    for(int i=0; i<filas; i++) {
        for(int j=0; j<columnas; j++) {
            matriz1D[i * columnas + j] = matriz[i][j];
        }
    }
    return matriz1D;
}

// Función para convertir matriz 1D a 2D desde memoria compartida
int **matriz1Da2D(int *matriz1D, int filas, int columnas) {
    int **matriz2D = (int **)malloc(filas * sizeof(int *));
    for(int i=0; i<filas; i++) {
        matriz2D[i] = (int *)malloc(columnas * sizeof(int));
        for(int j=0; j<columnas; j++) {
            matriz2D[i][j] = matriz1D[i * columnas + j];
        }
    }
    return matriz2D;
}

// Función que ejecutará cada proceso para procesar un conjunto de filas
void procesarFilasProceso(DatosProceso datos) {
    // Inicializar el contador de operaciones para este proceso
    datos.operaciones = 0;
    
    // Procesar las filas asignadas a este proceso
    for(int i=datos.fila_inicio; i<datos.fila_fin; i++){
        for(int j=0; j<datos.columnas_B; j++){
            // Inicializar el elemento de la matriz resultado
            datos.matriz_resultado[i * datos.columnas_B + j] = 0;
            
            // Calcular el producto punto de la fila i de A y la columna j de B
            for(int k=0; k<datos.columnas_A; k++){
                datos.matriz_resultado[i * datos.columnas_B + j] += 
                    datos.matriz_A[i * datos.columnas_A + k] * 
                    datos.matriz_B[k * datos.columnas_B + j];
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
    
    // Convertir matrices a formato 1D para memoria compartida
    int *matriz_A_1D = matriz2Da1D(matriz_A, filas_A, columnas_A);
    int *matriz_B_1D = matriz2Da1D(matriz_B, filas_B, columnas_B);
    int *matriz_resultado_1D = (int *)malloc(filas_A * columnas_B * sizeof(int));
    
    // Crear memoria compartida para las matrices
    int shmid_A, shmid_B, shmid_resultado;
    key_t key_A = 1234, key_B = 5678, key_resultado = 9012;
    
    // Crear segmentos de memoria compartida
    shmid_A = shmget(key_A, filas_A * columnas_A * sizeof(int), IPC_CREAT | 0666);
    shmid_B = shmget(key_B, filas_B * columnas_B * sizeof(int), IPC_CREAT | 0666);
    shmid_resultado = shmget(key_resultado, filas_A * columnas_B * sizeof(int), IPC_CREAT | 0666);
    
    // Adjuntar las matrices a la memoria compartida
    int *matriz_A_shared = (int *)shmat(shmid_A, NULL, 0);
    int *matriz_B_shared = (int *)shmat(shmid_B, NULL, 0);
    int *matriz_resultado_shared = (int *)shmat(shmid_resultado, NULL, 0);
    
    // Copiar datos a memoria compartida
    memcpy(matriz_A_shared, matriz_A_1D, filas_A * columnas_A * sizeof(int));
    memcpy(matriz_B_shared, matriz_B_1D, filas_B * columnas_B * sizeof(int));
    memset(matriz_resultado_shared, 0, filas_A * columnas_B * sizeof(int));
    
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
        
        // Pasar los punteros a las matrices en memoria compartida
        datos_procesos[i].matriz_A = matriz_A_shared;
        datos_procesos[i].matriz_B = matriz_B_shared;
        datos_procesos[i].matriz_resultado = matriz_resultado_shared;
        
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
    
    // Copiar el resultado de la memoria compartida a la matriz local
    memcpy(matriz_resultado_1D, matriz_resultado_shared, filas_A * columnas_B * sizeof(int));
    
    // Convertir el resultado a formato 2D
    int **resultado_temp = matriz1Da2D(matriz_resultado_1D, filas_A, columnas_B);
    
    // Copiar el resultado a la matriz original
    for(int i=0; i<filas_A; i++) {
        for(int j=0; j<columnas_B; j++) {
            matriz_resultado[i][j] = resultado_temp[i][j];
        }
    }
    
    // Liberar memoria temporal
    free(matriz_A_1D);
    free(matriz_B_1D);
    free(matriz_resultado_1D);
    for(int i=0; i<filas_A; i++) free(resultado_temp[i]);
    free(resultado_temp);
    
    // Desvincular y eliminar memoria compartida
    shmdt(matriz_A_shared);
    shmdt(matriz_B_shared);
    shmdt(matriz_resultado_shared);
    shmctl(shmid_A, IPC_RMID, NULL);
    shmctl(shmid_B, IPC_RMID, NULL);
    shmctl(shmid_resultado, IPC_RMID, NULL);
    
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


# Multiplicación de Matrices con Procesos y Memoria Compartida: Guía Detallada

## Introducción

Este documento explica en detalle una implementación de la multiplicación de matrices utilizando múltiples procesos y memoria compartida en sistemas operativos tipo Unix. La implementación demuestra conceptos fundamentales de sistemas operativos como creación de procesos, comunicación entre procesos (IPC) mediante memoria compartida, y sincronización.

## Conceptos Fundamentales

### Procesos
Un proceso es una instancia de un programa en ejecución que tiene su propio espacio de direcciones, registros, pila y código. A diferencia de los hilos, los procesos no comparten memoria de forma automática, lo que requiere mecanismos explícitos de comunicación entre procesos (IPC).

### Memoria Compartida
La memoria compartida es un mecanismo de IPC que permite a múltiples procesos acceder a la misma región de memoria. Es uno de los métodos más rápidos de IPC porque los procesos pueden leer y escribir datos directamente en memoria sin necesidad de copiarlos entre espacios de direcciones.

### Fork y Waitpid
- `fork()`: Crea un nuevo proceso duplicando el proceso actual. El proceso original se llama "padre" y el nuevo "hijo".
- `waitpid()`: Hace que el proceso padre espere a que un proceso hijo específico termine su ejecución.

## Estructura General del Código

El programa se organiza en varias secciones principales:

1. **Estructura de datos para procesos (DatosProceso)**
2. **Función principal (main)**
3. **Función que ejecuta cada proceso (procesarFilasProceso)**
4. **Función de multiplicación con procesos (multiplicarMatrizProcesos)**
5. **Funciones auxiliares (inicializarMatriz, verMatriz, verConsumoMemoria)**
6. **Funciones de conversión de matrices (matriz2Da1D, matriz1Da2D)**

## Estructura de Datos para Procesos (DatosProceso)

```c
typedef struct {
    int id_proceso;             // Identificador del proceso
    int fila_inicio;            // Fila inicial que procesará este proceso
    int fila_fin;               // Fila final que procesará este proceso
    int filas_A;                // Número de filas de la matriz A
    int columnas_A;             // Número de columnas de la matriz A
    int columnas_B;             // Número de columnas de la matriz B
    int *matriz_A;              // Puntero a la matriz A (en memoria compartida)
    int *matriz_B;              // Puntero a la matriz B (en memoria compartida)
    int *matriz_resultado;      // Puntero a la matriz resultado (en memoria compartida)
    long long operaciones;      // Contador de operaciones realizadas por este proceso
} DatosProceso;
```

### Explicación:

Esta estructura encapsula toda la información que un proceso necesita para realizar su trabajo:

- **id_proceso**: Identificador único para cada proceso, útil para depuración y seguimiento.
- **fila_inicio y fila_fin**: Definen el rango de filas de la matriz resultado que este proceso debe calcular.
- **Dimensiones de matrices (filas_A, columnas_A, columnas_B)**: Necesarias para los bucles de cálculo.
- **Punteros a matrices**: A diferencia de la versión con hilos, estos punteros apuntan a memoria compartida, no a memoria local.
- **operaciones**: Contador local de operaciones (multiplicaciones y sumas) realizadas por este proceso.

## Función Principal (main)

### Lectura y Validación de Argumentos

```c
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
```

### Explicación:

- **Validación de argumentos**: El programa espera 5 argumentos: dimensiones de las matrices y número de procesos.
- **Conversión a enteros**: Los argumentos de línea de comandos son cadenas, se convierten a enteros con `atoi`.
- **Compatibilidad de matrices**: Para multiplicar matrices, el número de columnas de la primera debe ser igual al número de filas de la segunda.
- **Validación de procesos**: Se asegura que el número de procesos sea positivo y establece un límite máximo (20) para evitar sobrecargar el sistema.

### Reserva de Memoria para las Matrices

```c
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
```

### Explicación:

- **Representación de matrices**: Las matrices se implementan como punteros dobles. Primero se reserva un array de punteros (uno por fila), y luego para cada fila se reserva memoria para las columnas.
- **Matriz resultado**: Tiene dimensiones `filas_A x columnas_B`, que es el resultado de multiplicar una matriz `filas_A x columnas_A` por una `filas_B x columnas_B`.

### Inicialización y Ejecución

```c
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
```

### Explicación:

- **Inicialización**: Se llena las matrices con valores aleatorios entre 0 y 9.
- **Medición de tiempo**: Se utiliza `clock()` para medir el tiempo de CPU antes y después de la multiplicación.
- **Llamada a la función con procesos**: Se invoca a `multiplicarMatrizProcesos` con todos los parámetros necesarios, incluyendo el número de procesos.

### Impresión de Resultados y Liberación de Memoria

```c
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
```

### Explicación:

- **Impresión de matrices**: Se muestran las matrices de entrada y el resultado (solo recomendable para matrices pequeñas).
- **Estadísticas de rendimiento**: Se muestran las dimensiones, tiempo de ejecución, consumo de memoria, total de operaciones y número de procesos utilizados.
- **Liberación de memoria**: Se libera la memoria en orden inverso a la reserva: primero los arrays de cada fila y luego el array de punteros.

## 3. Funciones de Conversión de Matrices

### 3.1. Conversión de 2D a 1D

```c
int *matriz2Da1D(int **matriz, int filas, int columnas) {
    int *matriz1D = (int *)malloc(filas * columnas * sizeof(int));
    for(int i=0; i<filas; i++) {
        for(int j=0; j<columnas; j++) {
            matriz1D[i * columnas + j] = matriz[i][j];
        }
    }
    return matriz1D;
}
```

### Explicación:

Esta función convierte una matriz 2D (representada como puntero a punteros) a un array 1D contiguo en memoria:

- **Reserva de memoria**: Se reserva un bloque contiguo de memoria para todos los elementos de la matriz.
- **Copia de datos**: Se recorre la matriz 2D y se copian los elementos al array 1D usando aritmética de punteros.
- **Índices**: El elemento `[i][j]` en la matriz 2D se almacena en la posición `i * columnas + j` del array 1D.

Esta conversión es necesaria porque la memoria compartida funciona mejor con bloques contiguos de memoria.

### Conversión de 1D a 2D

```c
int **matriz1Da2D(int *matriz1D, int filas, int columnas) {
    int **matriz2D = (int **)malloc(filas * sizeof(int *));
    for(int i=0; i<filas; i++) {
        matriz2D[i] = (int *)malloc(columnas * sizeof(int));
        for(int j=0; j<columnas; j++) {
            matriz2D[i][j] = matriz1D[i * columnas + j];
        }
    }
    return matriz2D;
}
```

### Explicación:

Esta función realiza la operación inversa, convirtiendo un array 1D a una matriz 2D:

- **Reserva de memoria para punteros**: Se reserva un array de punteros (uno por fila).
- **Reserva de memoria para filas**: Para cada fila, se reserva memoria para las columnas.
- **Copia de datos**: Se copian los elementos del array 1D a la matriz 2D usando la misma fórmula de indexación.

Esta función se utiliza para convertir los resultados de la memoria compartida (que está en formato 1D) a una matriz 2D que pueda ser utilizada por el resto del programa.

## Función que Ejecuta Cada Proceso (procesarFilasProceso)

```c
void procesarFilasProceso(DatosProceso datos) {
    // Inicializar el contador de operaciones para este proceso
    datos.operaciones = 0;
    
    // Procesar las filas asignadas a este proceso
    for(int i=datos.fila_inicio; i<datos.fila_fin; i++){
        for(int j=0; j<datos.columnas_B; j++){
            // Inicializar el elemento de la matriz resultado
            datos.matriz_resultado[i * datos.columnas_B + j] = 0;
            
            // Calcular el producto punto de la fila i de A y la columna j de B
            for(int k=0; k<datos.columnas_A; k++){
                datos.matriz_resultado[i * datos.columnas_B + j] += 
                    datos.matriz_A[i * datos.columnas_A + k] * 
                    datos.matriz_B[k * datos.columnas_B + j];
                // Incrementar el contador de operaciones (una multiplicación y una suma)
                datos.operaciones += 2;
            }
        }
    }
    
    // Salir del proceso con el número de operaciones como código de salida
    exit(datos.operaciones);
}
```

### Explicación Detallada:

Esta función es el corazón de la computación en cada proceso:

1. **Inicialización del contador local**:
   - Cada proceso tiene su propio contador de operaciones.
   - Se inicializa a cero al comienzo de la ejecución.

2. **Procesamiento de filas asignadas**:
   - El proceso procesa solo las filas desde `fila_inicio` hasta `fila_fin-1`.
   - Esta es la clave del paralelismo: cada proceso trabaja en un subconjunto de datos diferente.

3. **Cálculo del producto punto**:
   - Para cada elemento `[i][j]` de la matriz resultado, se calcula el producto punto de la fila `i` de la matriz A y la columna `j` de la matriz B.
   - **Importante**: Como las matrices están en memoria compartida en formato 1D, se utiliza aritmética de punteros para acceder a los elementos:
     - `datos.matriz_A[i * datos.columnas_A + k]` accede al elemento `[i][k]` de la matriz A.
     - `datos.matriz_B[k * datos.columnas_B + j]` accede al elemento `[k][j]` de la matriz B.
     - `datos.matriz_resultado[i * datos.columnas_B + j]` accede al elemento `[i][j]` de la matriz resultado.

4. **Conteo de operaciones**:
   - Cada multiplicación y suma se cuenta como dos operaciones.
   - Como cada proceso tiene su propio contador, no hay condiciones de carrera.

5. **Finalización del proceso**:
   - `exit(datos.operaciones)` termina el proceso y devuelve el número de operaciones como código de salida.
   - El proceso padre puede recuperar este valor usando `waitpid` y `WEXITSTATUS`.

## Función de Multiplicación con Procesos (multiplicarMatrizProcesos)

Esta es la función más compleja y la que implementa la lógica de concurrencia:

### Preparación Inicial

```c
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
```

### Explicación:

- **Reserva de memoria para identificadores de procesos**: Se reserva un array para almacenar los PIDs de los procesos hijos.
- **Reserva de memoria para datos de procesos**: Se reserva un array de estructuras `DatosProceso`, una por proceso.
- **Cálculo de distribución de trabajo**: `filas_por_proceso = filas_A / num_procesos` calcula cuántas filas le corresponden a cada proceso.
- **Inicialización del contador total**: Se inicializa el contador total de operaciones a cero.

### Conversión de Matrices a Formato de una dimensión

```c
    // Convertir matrices a formato 1D para memoria compartida
    int *matriz_A_1D = matriz2Da1D(matriz_A, filas_A, columnas_A);
    int *matriz_B_1D = matriz2Da1D(matriz_B, filas_B, columnas_B);
    int *matriz_resultado_1D = (int *)malloc(filas_A * columnas_B * sizeof(int));
```

### Explicación:

- **Conversión de matrices A y B**: Se convierten las matrices de entrada de formato 2D a 1D para poder almacenarlas en memoria compartida.
- **Reserva de memoria para resultado 1D**: Se reserva un array 1D para la matriz resultado, que se inicializará en memoria compartida.

### Creación de Memoria Compartida

```c
    // Crear memoria compartida para las matrices
    int shmid_A, shmid_B, shmid_resultado;
    key_t key_A = 1234, key_B = 5678, key_resultado = 9012;
    
    // Crear segmentos de memoria compartida
    shmid_A = shmget(key_A, filas_A * columnas_A * sizeof(int), IPC_CREAT | 0666);
    shmid_B = shmget(key_B, filas_B * columnas_B * sizeof(int), IPC_CREAT | 0666);
    shmid_resultado = shmget(key_resultado, filas_A * columnas_B * sizeof(int), IPC_CREAT | 0666);
```

### Explicación:

- **Identificadores de memoria compartida**: `shmid_A`, `shmid_B`, y `shmid_resultado` almacenarán los identificadores de los segmentos de memoria compartida.
- **Claves de memoria compartida**: `key_A`, `key_B`, y `key_resultado` son claves únicas que identifican cada segmento de memoria compartida. En un entorno real, estas claves deberían generarse de forma más robusta.
- **Creación de segmentos**: `shmget` crea un segmento de memoria compartida con:
  - Una clave única.
  - El tamaño necesario para almacenar la matriz.
  - `IPC_CREAT | 0666` indica que cree el segmento si no existe y con permisos de lectura/escritura para todos.

### Adjuntar Memoria Compartida y Copiar Datos

```c
    // Adjuntar las matrices a la memoria compartida
    int *matriz_A_shared = (int *)shmat(shmid_A, NULL, 0);
    int *matriz_B_shared = (int *)shmat(shmid_B, NULL, 0);
    int *matriz_resultado_shared = (int *)shmat(shmid_resultado, NULL, 0);
    
    // Copiar datos a memoria compartida
    memcpy(matriz_A_shared, matriz_A_1D, filas_A * columnas_A * sizeof(int));
    memcpy(matriz_B_shared, matriz_B_1D, filas_B * columnas_B * sizeof(int));
    memset(matriz_resultado_shared, 0, filas_A * columnas_B * sizeof(int));
```

### Explicación:

- **Adjuntar memoria compartida**: `shmat` adjunta el segmento de memoria compartida al espacio de direcciones del proceso:
  - El primer argumento es el identificador del segmento.
  - El segundo argumento (NULL) indica que el sistema elija la dirección.
  - El tercer argumento (0) indica flags de acceso (lectura/escritura).
- **Copiar datos a memoria compartida**:
  - `memcpy` copia los datos de las matrices locales a la memoria compartida.
  - `memset` inicializa la matriz resultado en memoria compartida a cero.

### Creación de Procesos

```c
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
        
        // Pasar los punteros a las matrices en memoria compartida
        datos_procesos[i].matriz_A = matriz_A_shared;
        datos_procesos[i].matriz_B = matriz_B_shared;
        datos_procesos[i].matriz_resultado = matriz_resultado_shared;
        
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
```

### Explicación:

- **Configuración de datos para cada proceso**:
  - Se asigna un rango de filas a cada proceso.
  - Si el número de filas no es divisible entre el número de procesos, el último proceso toma las filas restantes.
  - Se pasan las dimensiones de las matrices y los punteros a la memoria compartida.
- **Creación de procesos**:
  - `fork()` crea un nuevo proceso:
    - Si devuelve 0, estamos en el proceso hijo, que ejecuta `procesarFilasProceso`.
    - Si devuelve un valor positivo, estamos en el proceso padre, y el valor es el PID del hijo.
    - Si devuelve un valor negativo, hubo un error al crear el proceso.
  - El proceso padre continúa el bucle para crear más procesos, mientras que cada proceso hijo ejecuta su tarea y termina.

### Espera y Recopilación de Resultados

```c
    // Esperar a que todos los procesos terminen
    int estado;
    for(int i=0; i<num_procesos; i++) {
        waitpid(procesos[i], &estado, 0);
        // Obtener el número de operaciones del código de salida
        if (WIFEXITED(estado)) {
            *operaciones += WEXITSTATUS(estado);
        }
    }
```

### Explicación:

- **Espera a los procesos hijos**: `waitpid` hace que el proceso padre espere a que el proceso hijo con el PID especificado termine.
- **Recopilación de resultados**:
  - `WIFEXITED(estado)` comprueba si el proceso terminó normalmente.
  - `WEXITSTATUS(estado)` obtiene el valor de retorno del proceso (el número de operaciones en nuestro caso).
  - Se suman las operaciones de todos los procesos para obtener el total.

### Recuperación de Resultados y Limpieza

```c
    // Copiar el resultado de la memoria compartida a la matriz local
    memcpy(matriz_resultado_1D, matriz_resultado_shared, filas_A * columnas_B * sizeof(int));
    
    // Convertir el resultado a formato 2D
    int **resultado_temp = matriz1Da2D(matriz_resultado_1D, filas_A, columnas_B);
    
    // Copiar el resultado a la matriz original
    for(int i=0; i<filas_A; i++) {
        for(int j=0; j<columnas_B; j++) {
            matriz_resultado[i][j] = resultado_temp[i][j];
        }
    }
    
    // Liberar memoria temporal
    free(matriz_A_1D);
    free(matriz_B_1D);
    free(matriz_resultado_1D);
    for(int i=0; i<filas_A; i++) free(resultado_temp[i]);
    free(resultado_temp);
    
    // Desvincular y eliminar memoria compartida
    shmdt(matriz_A_shared);
    shmdt(matriz_B_shared);
    shmdt(matriz_resultado_shared);
    shmctl(shmid_A, IPC_RMID, NULL);
    shmctl(shmid_B, IPC_RMID, NULL);
    shmctl(shmid_resultado, IPC_RMID, NULL);
    
    // Liberar memoria reservada para los procesos y datos
    free(procesos);
    free(datos_procesos);
}
```

### Explicación:

- **Recuperación de resultados**:
  - Se copia el resultado de la memoria compartida a un array 1D local.
  - Se convierte el array 1D a una matriz 2D.
  - Se copian los datos a la matriz resultado original.
- **Liberación de memoria temporal**: Se liberan los arrays 1D y la matriz temporal.
- **Liberación de memoria compartida**:
  - `shmdt` desvincula la memoria compartida del espacio de direcciones del proceso.
  - `shmctl` con `IPC_RMID` elimina el segmento de memoria compartida.
- **Liberación de memoria de procesos**: Se liberan los arrays de identificadores de procesos y datos de procesos.

## Funciones Auxiliares

### Inicialización de Matriz

```c
void inicializarMatriz(int filas, int columnas, int **matriz){
   for(int i=0; i<filas; i++){
    for(int j=0; j<columnas; j++){
        matriz[i][j] = rand() % 10;
    }
   }
}
```

### Explicación:

- Llena la matriz con valores aleatorios entre 0 y 9.
- Utiliza `rand()` que genera números pseudoaleatorios.
- El operador módulo (`% 10`) limita los valores al rango 0-9.

### Impresión de Matriz

```c
void verMatriz(int filas, int columnas, int **matriz){
    for(int i=0; i<filas; i++){
        for(int j=0; j<columnas; j++){
            printf("%d\t", matriz[i][j]);
        }
        printf("\n");
   }
}
```

### Explicación:

- Imprime la matriz en formato tabular.
- Utiliza tabuladores (`\t`) para alinear las columnas.
- Imprime un salto de línea (`\n`) después de cada fila.

### Obtención de Consumo de Memoria

```c
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

### Explicación:

- Lee el archivo `/proc/self/status` (disponible en sistemas Linux) para obtener el consumo de memoria física (RSS - Resident Set Size) del proceso.
- Busca la línea que comienza con "VmRSS:" y extrae el valor en KB.
- Este método es específico de Linux y proporciona una medida aproximada del uso de memoria.

## Conceptos Clave de Sistemas Operativos (procesos)

### Procesos y Fork

- **fork()**: Es la llamada al sistema fundamental para crear nuevos procesos en Unix.
  - Crea una copia exacta del proceso padre, incluyendo el espacio de direcciones.
  - El proceso hijo comienza a ejecutarse desde el punto después de la llamada a fork().
  - La única diferencia es el valor de retorno: 0 para el hijo, PID del hijo para el padre.
- **Espacio de direcciones independiente**: A diferencia de los hilos, cada proceso tiene su propio espacio de direcciones, lo que proporciona aislamiento pero requiere mecanismos explícitos para compartir datos.

### Memoria Compartida

- **shmget()**: Crea o obtiene un segmento de memoria compartida.
  - Requiere una clave única, un tamaño y flags.
  - Devuelve un identificador del segmento.
- **shmat()**: Adjunta un segmento de memoria compartida al espacio de direcciones del proceso.
  - Devuelve un puntero a la memoria compartida.
  - Permite leer y escribir directamente en la memoria compartida.
- **shmdt()**: Desvincula un segmento de memoria compartida del espacio de direcciones del proceso.
- **shmctl()**: Realiza operaciones de control sobre un segmento de memoria compartida.
  - Con `IPC_RMID`, elimina el segmento.

### Comunicación entre Procesos (IPC)

La memoria compartida es uno de varios mecanismos de IPC en Unix:

- **Memoria compartida**: La más rápida, pero requiere sincronización explícita.
- **Pipes**: Unidireccionales, para comunicación entre procesos relacionados.
- **Colas de mensajes**: Más estructuradas, pero con mayor overhead.
- **Sockets**: Para comunicación entre procesos en diferentes máquinas.

### Sincronización de Procesos

- **waitpid()**: Permite a un proceso esperar a que otro proceso termine.
  - Esencial para sincronizar el proceso padre con los hijos.
  - Permite obtener el estado de terminación del hijo.
- **Códigos de salida**: Los procesos pueden devolver un valor al terminar usando `exit()`.
  - El proceso padre puede obtener este valor usando `WEXITSTATUS()`.

---
## Comparación: Hilos vs Procesos

### Diferencias Fundamentales

| Aspecto                | Hilos                                  | Procesos                                |
|-----------------------|----------------------------------------|-----------------------------------------|
| Espacio de direcciones | Compartido                             | Independiente                            |
| Creación              | Rápida (pthread_create)                | Lenta (fork)                            |
| Comunicación          | Simple (variables compartidas)          | Compleja (requiere IPC)                  |
| Sincronización        | Mutexes, variables condición            | Más compleja (semáforos, etc.)           |
| Aislamiento           | Bajo (un error afecta a todo el proceso)| Alto (procesos independientes)          |
| Robustez              | Menor                                  | Mayor                                   |
| Consumo de recursos   | Menor                                  | Mayor                                   |
| Cambio de contexto    | Rápido                                 | Lento                                   |

### Ventajas de los Procesos

1. **Aislamiento**: Cada proceso tiene su propio espacio de direcciones, lo que proporciona mayor seguridad y robustez.
2. **Robustez**: Si un proceso falla, los demás pueden continuar ejecutándose.
3. **Distribución**: Los procesos pueden ejecutarse en diferentes máquinas de una red.
4. **Paralelismo real**: En sistemas con múltiples procesadores, los procesos pueden ejecutarse realmente en paralelo.

### Desventajas de los Procesos

1. **Overhead de creación**: Crear un proceso es más costoso que crear un hilo.
2. **Comunicación compleja**: Requiere mecanismos explícitos de IPC.
3. **Mayor consumo de recursos**: Cada proceso consume más memoria y otros recursos del sistema.
4. **Cambio de contexto más lento**: Cambiar entre procesos es más costoso que cambiar entre hilos.

### Cuándo Usar Procesos

1. Cuando se requiere mayor robustez y aislamiento.
2. Cuando las tareas son independientes y no necesitan comunicarse frecuentemente.
3. Cuando se trabaja en un entorno distribuido.
4. Cuando la seguridad es crítica.


## Conclusiones respecto a procesos

Esta implementación de multiplicación de matrices con procesos y memoria compartida demuestra varios conceptos fundamentales de sistemas operativos:

1. **Creación y gestión de procesos**: Uso de `fork()` y `waitpid()` para crear y sincronizar procesos.
2. **Comunicación entre procesos**: Implementación de memoria compartida para compartir datos entre procesos.
3. **Conversión de datos**: Transformación entre representaciones 2D y 1D para facilitar el uso de memoria compartida.
4. **Sincronización**: Uso de códigos de salida y `waitpid()` para recopilar resultados.
5. **Gestión de recursos**: Liberación adecuada de memoria compartida y otros recursos.

### Lecciones a considerar

1. **La memoria compartida es poderosa pero compleja**: Requiere gestión cuidadosa para evitar fugas de memoria.
2. **Los procesos proporcionan aislamiento pero requieren más recursos**: La creación y gestión de procesos es más costosa que la de hilos.
3. **La comunicación entre procesos es explícita**: A diferencia de los hilos, los procesos no comparten memoria automáticamente.
4. **La sincronización es esencial**: Es necesario esperar a que todos los procesos terminen antes de usar los resultados.

### Aplicaciones Prácticas

Este tipo de implementación es útil en:

1. **Sistemas distribuidos**: Donde las tareas deben ejecutarse en diferentes máquinas.
2. **Aplicaciones críticas**: Donde la robustez y el aislamiento son más importantes que el rendimiento.
3. **Procesamiento paralelo**: En sistemas con múltiples procesadores o núcleos.
4. **Sistemas con requisitos de seguridad**: Donde el aislamiento entre tareas es fundamental.

### Posibles Mejoras

1. **Generación de claves más robusta**: En lugar de usar claves fijas, se podrían generar dinámicamente.
2. **Manejo de errores más completo**: Verificar todas las llamadas al sistema y manejar los errores adecuadamente.
3. **Balanceo de carga dinámico**: En lugar de asignar estáticamente las filas, se podría usar una cola de tareas.
4. **Sincronización más fina**: Usar semáforos para sincronizar el acceso a regiones críticas si fuera necesario.


---
