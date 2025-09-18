# Concurrencia en Multiplicación de Matrices usando Hilos

## Código con Concurrencia por Hilos

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>
#include <pthread.h>

// Número de hilos a utilizar
#define NUM_HILOS 4

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
                                  int **matriz_resultado, long long *operaciones);

// Función para inicializar matriz con valores aleatorios
void inicializarMatriz(int filas, int columnas, int **matriz);

// Función para imprimir matriz en pantalla
void verMatriz(int filas, int columnas, int **matriz);

// Función para obtener consumo de memoria del proceso
long verConsumoMemoria();

int main(int argc, char *argv[]){    
    // Verificar argumentos de entrada
    if (argc != 5) {
        printf("Uso: %s filas_A columnas_A filas_B columnas_B\n", argv[0]);
        return -1;
    }
    
    // Convertir argumentos a enteros
    int filas_A = atoi(argv[1]);
    int columnas_A = atoi(argv[2]);
    int filas_B = atoi(argv[3]);
    int columnas_B = atoi(argv[4]);

    // Verificar compatibilidad de matrices para multiplicación
    if (columnas_A != filas_B){
        printf("Error: Columnas de A (%d) deben ser iguales a filas de B (%d)\n", 
               columnas_A, filas_B);
        return -1;
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
                                 matriz_resultado, &operaciones);
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
    printf("Número de hilos utilizados: %d\n", NUM_HILOS);
 
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
                                  int **matriz_resultado, long long *operaciones) {
    
    // Arreglo para almacenar los identificadores de los hilos
    pthread_t hilos[NUM_HILOS];
    
    // Arreglo para almacenar los datos de cada hilo
    DatosHilo datos_hilos[NUM_HILOS];
    
    // Calcular cuántas filas procesará cada hilo
    int filas_por_hilo = filas_A / NUM_HILOS;
    
    // Inicializar el contador total de operaciones
    *operaciones = 0;
    
    // Crear los hilos
    for(int i=0; i<NUM_HILOS; i++) {
        // Configurar los datos para este hilo
        datos_hilos[i].id_hilo = i;
        datos_hilos[i].fila_inicio = i * filas_por_hilo;
        datos_hilos[i].fila_fin = (i+1) * filas_por_hilo;
        
        // Si es el último hilo, asignarle las filas restantes
        if(i == NUM_HILOS-1) {
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
    for(int i=0; i<NUM_HILOS; i++) {
        pthread_join(hilos[i], NULL);
        // Sumar las operaciones realizadas por este hilo al total
        *operaciones += datos_hilos[i].operaciones;
    }
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

## Explicación del Código Concurrente

### Introducción a la Concurrencia por Hilos

La concurrencia es una técnica que permite ejecutar múltiples tareas simultáneamente, aprovechando los múltiples núcleos de los procesadores modernos. En el caso de la multiplicación de matrices, esta técnica es especialmente útil porque:

1. Cada elemento de la matriz resultado se calcula independientemente de los demás
2. No hay dependencias entre los cálculos de diferentes elementos
3. Es una operación computacionalmente intensiva que se beneficia del paralelismo

### Implementación con Hilos

Nuestra implementación utiliza la biblioteca pthreads de POSIX para crear y gestionar hilos. El enfoque principal es dividir el trabajo entre varios hilos, donde cada hilo se encarga de calcular un conjunto de filas de la matriz resultado.

#### Estructura de Datos para Hilos

Hemos definido una estructura `DatosHilo` que contiene toda la información que un hilo necesita para realizar su trabajo:

```c
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
```

Esta estructura nos permite pasar de manera organizada todos los datos necesarios a cada hilo.

#### Función del Hilo

La función `procesarFilas` es la que ejecuta cada hilo. Recibe un puntero a una estructura `DatosHilo` y realiza los siguientes pasos:

1. Convierte el argumento a la estructura de datos
2. Inicializa su contador local de operaciones
3. Procesa las filas asignadas (desde `fila_inicio` hasta `fila_fin-1`)
4. Para cada fila, calcula todos los elementos de esa fila en la matriz resultado
5. Finaliza el hilo con `pthread_exit`

#### Función Principal de Multiplicación Concurrente

La función `multiplicarMatrizConcurrente` es la responsable de:

1. Crear los hilos necesarios (definidos por la constante `NUM_HILOS`)
2. Dividir las filas de la matriz resultado entre los hilos
3. Configurar los datos para cada hilo
4. Iniciar la ejecución de los hilos con `pthread_create`
5. Esperar a que todos los hilos terminen con `pthread_join`
6. Sumar las operaciones realizadas por cada hilo para obtener el total

La distribución de filas se hace de manera equitativa. Si el número de filas no es divisible entre el número de hilos, el último hilo se encarga de las filas restantes.

### Ventajas de este Enfoque

1. **Simplicidad**: La implementación es relativamente sencilla de entender y mantener
2. **Escalabilidad**: Al aumentar el número de núcleos, podemos aumentar el número de hilos para mejorar el rendimiento
3. **Balance de carga**: Cada hilo procesa aproximadamente el mismo número de filas, lo que distribuye el trabajo de manera equitativa
4. **Mínima sincronización**: Los hilos trabajan en diferentes regiones de la matriz resultado, por lo que no necesitan sincronizarse durante el cálculo

### Consideraciones de Rendimiento

1. **Número óptimo de hilos**: Generalmente, el número óptimo de hilos es igual al número de núcleos del procesador. Un número mayor puede generar sobrecarga por la gestión de hilos.
2. **Tamaño de las matrices**: Para matrices pequeñas, el overhead de crear y gestionar hilos puede superar los beneficios del paralelismo.
3. **Localidad de datos**: Cada hilo accede a todas las matrices de entrada, lo que puede generar conflictos en la caché. Para matrices muy grandes, podría ser beneficioso transponer la matriz B para mejorar la localidad de datos.

### Posibles Mejoras

1. **Ajuste dinámico del número de hilos**: Podríamos detectar el número de núcleos disponibles y ajustar `NUM_HILOS` en consecuencia.
2. **Transposición de la matriz B**: Esto mejoraría la localidad de datos y podría aumentar el rendimiento.
3. **Paralelización anidada**: Para sistemas con muchos núcleos, podríamos combinar paralelización a nivel de filas con paralelización dentro de cada fila.
4. **Balance de carga dinámico**: En lugar de asignar estáticamente las filas, podríamos usar una cola de tareas donde los hilos toman filas según terminan su trabajo actual.

### Conclusión

La implementación de concurrencia mediante hilos en la multiplicación de matrices ofrece una mejora significativa en el rendimiento, especialmente para matrices grandes. Nuestra implementación divide el trabajo de manera equitativa entre los hilos, permitiendo aprovechar los múltiples núcleos de los procesadores modernos. Este enfoque es un excelente punto de partida para optimizaciones más avanzadas.
```
