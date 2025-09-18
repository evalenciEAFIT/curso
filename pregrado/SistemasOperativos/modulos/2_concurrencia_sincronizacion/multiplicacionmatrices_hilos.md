# Concurrencia en Multiplicación de Matrices usando Hilos

## Código con Concurrencia por Hilos
[Regresar](readme.md)
Para modificar el código y que el número de hilos sea un argumento de línea de comandos (CLI), necesitas hacer los siguientes cambios:

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

## Explicación de los Cambios

### Modificaciones Principales

1. **Eliminación de la constante NUM_HILOS**:
   - Se eliminó la constante `#define NUM_HILOS 4` que fijaba el número de hilos.
   - Ahora el número de hilos se especifica como argumento en la línea de comandos.

2. **Modificación en la función main()**:
   - Ahora se espera 6 argumentos en lugar de 5: `./programa filas_A columnas_A filas_B columnas_B num_hilos`
   - Se añadió validación para el número de hilos:
     ```c
     int num_hilos = atoi(argv[5]);
     
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
     ```

3. **Actualización de la función multiplicarMatrizConcurrente()**:
   - Ahora recibe un parámetro adicional: `int num_hilos`
   - Se utiliza memoria dinámica para los arrays de hilos y datos:
     ```c
     pthread_t *hilos = (pthread_t *)malloc(num_hilos * sizeof(pthread_t));
     DatosHilo *datos_hilos = (DatosHilo *)malloc(num_hilos * sizeof(DatosHilo));
     ```
   - Se libera la memoria al final de la función:
     ```c
     free(hilos);
     free(datos_hilos);
     ```

4. **Actualización en la llamada a multiplicarMatrizConcurrente()**:
   - Se pasa el número de hilos como argumento:
     ```c
     multiplicarMatrizConcurrente(filas_A, columnas_A, matriz_A,
                                  filas_B, columnas_B, matriz_B,
                                  matriz_resultado, &operaciones, num_hilos);
     ```

### Ventajas de esta Modificación

1. **Flexibilidad**:
   - Permite experimentar con diferentes números de hilos para encontrar el óptimo según el hardware y el tamaño de las matrices.
   - Facilita la comparación de rendimiento entre diferentes configuraciones.

2. **Adaptabilidad**:
   - El programa se adapta automáticamente al número de núcleos disponibles en el sistema.
   - Permite ajustar el número de hilos según las características del problema.

3. **Control de recursos**:
   - Se añadió un límite máximo de 100 hilos para evitar sobrecargar el sistema.
   - Se valida que el número de hilos sea un valor positivo.

### Cómo Usar el Programa

Para ejecutar el programa con un número específico de hilos, utiliza el siguiente formato:

```bash
./multiplicacion_matrices filas_A columnas_A filas_B columnas_B num_hilos
```

Ejemplos:
```bash
# Usar 2 hilos
./multiplicacion_matrices 500 500 500 500 2

# Usar 4 hilos
./multiplicacion_matrices 1000 1000 1000 1000 4

# Usar 8 hilos
./multiplicacion_matrices 2000 2000 2000 2000 8
```

### Consideraciones de Rendimiento

1. **Número óptimo de hilos**:
   - Generalmente, el número óptimo de hilos es igual al número de núcleos del procesador.
   - Para sistemas con hiperthreading, puedes probar con el doble de núcleos.
   - Experimenta con diferentes valores para encontrar el óptimo para tu hardware específico.

2. **Tamaño de las matrices**:
   - Para matrices pequeñas, usar demasiados hilos puede degradar el rendimiento debido al overhead de creación y gestión de hilos.
   - Para matrices grandes, más hilos generalmente mejora el rendimiento hasta cierto punto.

3. **Balance de carga**:
   - El programa distribuye las filas de manera equitativa entre los hilos.
   - Si el número de filas no es divisible entre el número de hilos, el último hilo procesa las filas restantes.

Esta modificación hace que el programa sea más flexible y adaptable a diferentes entornos y requisitos, permitiendo un mejor aprovechamiento de los recursos del sistema.
