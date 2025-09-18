# Concurrencia en Multiplicación de Matrices usando Hilos

## Código con Concurrencia por Hilos
[Regresar](readme.md)   

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
---

### EXPLICACIÓN
# Explicación Detallada del Código de Multiplicación de Matrices con Concurrencia por Hilos

## Introducción

Explicación detallada del código que implementa la multiplicación de matrices utilizando concurrencia mediante hilos POSIX (pthreads). La implementación divide el trabajo entre múltiples hilos para aprovechar los múltiples núcleos de los procesadores modernos, mejorando significativamente el rendimiento para matrices grandes.

## Estructura General del Código

El código está organizado en varias secciones principales:

1. **Estructura de datos para hilos**
2. **Función principal (main)**
3. **Función que ejecuta cada hilo (procesarFilas)**
4. **Función de multiplicación concurrente (multiplicarMatrizConcurrente)**
5. **Funciones auxiliares (inicializarMatriz, verMatriz, verConsumoMemoria)**

## 1. Estructura de Datos para Hilos (DatosHilo)

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

### Explicación:

Esta estructura encapsula toda la información que un hilo necesita para realizar su trabajo:

- **id_hilo**: Identificador único para cada hilo, útil para depuración y seguimiento.
- **fila_inicio y fila_fin**: Definen el rango de filas de la matriz resultado que este hilo debe calcular.
- **Dimensiones de matrices (filas_A, columnas_A, columnas_B)**: Necesarias para los bucles de cálculo.
- **Punteros a matrices (matriz_A, matriz_B, matriz_resultado)**: Todos los hilos comparten las mismas matrices, pero cada hilo escribe en una región distinta de la matriz resultado.
- **operaciones**: Contador local de operaciones (multiplicaciones y sumas) realizadas por este hilo. Cada hilo tiene su propio contador para evitar condiciones de carrera.

Esta estructura es esencial porque las funciones de hilos en pthreads solo pueden recibir un puntero void como argumento, por lo que necesitamos una forma de pasar múltiples parámetros.

## 2. Función Principal (main)

### 2.1. Lectura y Validación de Argumentos

```c
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
```

### Explicación:

- **Validación de argumentos**: El programa espera 5 argumentos: dimensiones de las matrices y número de hilos. Si no se proporcionan, muestra un mensaje de uso y termina.
- **Conversión a enteros**: Los argumentos de línea de comandos son cadenas, se convierten a enteros con `atoi`.
- **Compatibilidad de matrices**: Para multiplicar matrices, el número de columnas de la primera debe ser igual al número de filas de la segunda.
- **Validación de hilos**: Se asegura que el número de hilos sea positivo y establece un límite máximo (100) para evitar sobrecargar el sistema.

### 2.2. Reserva de Memoria para las Matrices

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

### 2.3. Inicialización y Ejecución

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
    // Usar la versión concurrente de la multiplicación
    multiplicarMatrizConcurrente(filas_A, columnas_A, matriz_A,
                                 filas_B, columnas_B, matriz_B,
                                 matriz_resultado, &operaciones, num_hilos);
    fin = clock();
    tiempo_CPU_usado = ((double)(fin-inicio))/CLOCKS_PER_SEC;
```

### Explicación:

- **Inicialización**: Se llena las matrices con valores aleatorios entre 0 y 9.
- **Medición de tiempo**: Se utiliza `clock()` para medir el tiempo de CPU antes y después de la multiplicación. La diferencia dividida por `CLOCKS_PER_SEC` da el tiempo en segundos.
- **Llamada a la función concurrente**: Se invoca a `multiplicarMatrizConcurrente` con todos los parámetros necesarios, incluyendo el número de hilos.

### 2.4. Impresión de Resultados y Liberación de Memoria

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
```

### Explicación:

- **Impresión de matrices**: Se muestran las matrices de entrada y el resultado (solo recomendable para matrices pequeñas).
- **Estadísticas de rendimiento**: Se muestran las dimensiones, tiempo de ejecución, consumo de memoria, total de operaciones y número de hilos utilizados.
- **Liberación de memoria**: Se libera la memoria en orden inverso a la reserva: primero los arrays de cada fila y luego el array de punteros.

## 3. Función que Ejecuta Cada Hilo (procesarFilas)

```c
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
```

### Explicación Detallada:

Esta función es el corazón de la concurrencia. Cada hilo ejecuta esta función de forma independiente:

1. **Recepción de parámetros**:
   - El hilo recibe un puntero `void*` que se convierte a nuestro tipo `DatosHilo`.
   - Esto es necesario porque las funciones de hilos en pthreads deben tener el prototipo `void* (*)(void*)`.

2. **Inicialización del contador local**:
   - Cada hilo tiene su propio contador de operaciones para evitar condiciones de carrera.
   - Se inicializa a cero al comienzo de la ejecución.

3. **Procesamiento de filas asignadas**:
   - El hilo procesa solo las filas desde `fila_inicio` hasta `fila_fin-1`.
   - Esta es la clave del paralelismo: cada hilo trabaja en un subconjunto de datos diferente.

4. **Cálculo del producto punto**:
   - Para cada elemento `[i][j]` de la matriz resultado, se calcula el producto punto de la fila `i` de la matriz A y la columna `j` de la matriz B.
   - Este es el algoritmo clásico de multiplicación de matrices.

5. **Conteo de operaciones**:
   - Cada multiplicación y suma se cuenta como dos operaciones.
   - Como cada hilo tiene su propio contador, no hay condiciones de carrera.

6. **Finalización del hilo**:
   - `pthread_exit(NULL)` termina la ejecución del hilo.
   - El valor NULL indica que no se retorna ningún resultado.

## 4. Función de Multiplicación Concurrente (multiplicarMatrizConcurrente)

```c
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
```

### Explicación Detallada:

Esta función coordina la creación y sincronización de los hilos:

1. **Reserva de memoria para estructuras de hilos**:
   - Se reserva memoria para un array de identificadores de hilos (`pthread_t`).
   - Se reserva memoria para un array de estructuras `DatosHilo`, una por hilo.
   - Esto permite un número variable de hilos especificado en tiempo de ejecución.

2. **Cálculo de distribución de trabajo**:
   - `filas_por_hilo = filas_A / num_hilos` calcula cuántas filas le corresponden a cada hilo.
   - Esta es una estrategia de **balanceo de carga estático**, donde se asume que todas las filas requieren el mismo esfuerzo computacional.

3. **Configuración de datos para cada hilo**:
   - Para cada hilo, se asigna un rango de filas (`fila_inicio` y `fila_fin`).
   - Si el número de filas no es divisible entre el número de hilos, el último hilo toma las filas restantes.
   - También se asignan las dimensiones de las matrices y los punteros a las matrices.

4. **Creación de hilos**:
   - `pthread_create(&hilos[i], NULL, procesarFilas, &datos_hilos[i])` crea un nuevo hilo.
   - El hilo ejecuta la función `procesarFilas` con el argumento `&datos_hilos[i]`.
   - Los hilos comienzan a ejecutarse inmediatamente después de su creación.

5. **Sincronización con pthread_join**:
   - `pthread_join(hilos[i], NULL)` hace que el hilo principal espere a que el hilo `hilos[i]` termine.
   - Esto es esencial para asegurar que todos los cálculos se completen antes de continuar.
   - Después de que cada hilo termina, se suman sus operaciones al contador total.

6. **Liberación de memoria**:
   - Se libera la memoria reservada para los arrays de hilos y datos.
   - Esto evita fugas de memoria.

## 5. Funciones Auxiliares

### 5.1. Inicialización de Matriz

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

### 5.2. Impresión de Matriz

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

### 5.3. Obtención de Consumo de Memoria

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
--- 
## 6. Conceptos Clave de Sistemas Operativos

### 6.1. Hilos (Threads)

Un hilo es la unidad básica de ejecución dentro de un proceso. Características importantes:

- **Comparten recursos**: Todos los hilos de un proceso comparten el mismo espacio de direcciones (memoria, archivos, etc.).
- **Contexto propio**: Cada hilo tiene su propia pila, registros y estado de ejecución.
- **Menor overhead**: Crear y cambiar entre hilos es más rápido que con procesos.

### 6.2. Espacio de Direcciones Compartido

En nuestra implementación:

- Las matrices (matriz_A, matriz_B, matriz_resultado) están en el heap y son compartidas por todos los hilos.
- Cada hilo accede a diferentes regiones de matriz_resultado, evitando conflictos.
- No se necesitan mecanismos de exclusión mutua (mutex) porque no hay escrituras concurrentes a las mismas posiciones.

### 6.3. Balanceo de Carga

El balanceo de carga se refiere a cómo se distribuye el trabajo entre los hilos:

- **Estrategia estática**: La distribución se decide antes de ejecutar los hilos (nuestro caso).
  - Ventaja: Bajo overhead durante la ejecución.
  - Desventaja: Si las tareas tienen duraciones diferentes, algunos hilos pueden terminar antes.

- **Estrategia dinámica**: Las tareas se asignan a medida que los hilos quedan disponibles.
  - Ventaja: Mejor balanceo de carga.
  - Desventaja: Mayor overhead de comunicación.

### 6.4. Sincronización

La sincronización es crucial para asegurar que los hilos cooperen correctamente:

- **pthread_join**: Implementa una barrera de sincronización. El hilo que llama se bloquea hasta que el hilo objetivo termine.
- En nuestro código, el hilo principal espera a que todos los hilos terminen antes de mostrar los resultados.

### 6.5. Concurrencia vs Paralelismo

- **Concurrencia**: Múltiples tareas avanzan en períodos solapados (no necesariamente simultáneamente).
- **Paralelismo**: Múltiples tareas se ejecutan simultáneamente en múltiples núcleos.
- En nuestro código, logramos paralelismo real si el sistema tiene múltiples núcleos y el sistema operativo asigna los hilos a diferentes núcleos.

## 7. Análisis de Rendimiento

### 7.1. Métricas Importantes

1. **Speedup (Aceleración)**:
   ```
   Speedup = Tiempo_secuencial / Tiempo_concurrente
   ```
   - Mide cuántas veces más rápido es el código concurrente.
   - Idealmente, Speedup = Número de hilos.

2. **Eficiencia**:
   ```
   Eficiencia = Speedup / Número de hilos
   ```
   - Mide qué tan bien se utilizan los recursos.
   - Idealmente, Eficiencia = 1 (100%).

### 7.2. Factores que Afectan el Rendimiento

1. **Tamaño de las matrices**:
   - Matrices pequeñas: El overhead de crear hilos puede superar los beneficios.
   - Matrices grandes: Mejor aprovechamiento del paralelismo.

2. **Número de hilos**:
   - Demasiados pocos: No se aprovecha todo el potencial del hardware.
   - Demasiados muchos: Overhead excesivo y contención de recursos.

3. **Arquitectura del hardware**:
   - Número de núcleos físicos y lógicos.
   - Tamaño de la caché y jerarquía de memoria.

## 8. Posibles Mejoras

### 8.1. Balanceo Dinámico de Carga

En lugar de asignar estáticamente las filas, podríamos usar una cola de tareas:

```c
int siguiente_fila = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *procesarFilasDinamico(void *arg) {
    int fila;
    while (1) {
        pthread_mutex_lock(&mutex);
        fila = siguiente_fila++;
        pthread_mutex_unlock(&mutex);
        
        if (fila >= filas_A) break;
        
        // Procesar fila...
    }
    pthread_exit(NULL);
}
```

### 8.2. Transposición de la Matriz B

Para mejorar la localidad de referencia:

```c
// Transponer matriz B antes de la multiplicación
int **matriz_B_transpuesta = (int **)malloc(columnas_B * sizeof(int *));
for(int i=0; i < columnas_B; i++) {
    matriz_B_transpuesta[i] = (int *)malloc(filas_B * sizeof(int));
    for(int j=0; j < filas_B; j++) {
        matriz_B_transpuesta[i][j] = matriz_B[j][i];
    }
}

// Luego en el cálculo:
matriz_resultado[i][j] += matriz_A[i][k] * matriz_B_transpuesta[j][k];
```

### 8.3. Detección Automática del Número de Núcleos

En lugar de pedir el número de hilos, podríamos detectarlo automáticamente:

```c
#include <unistd.h>
int num_hilos = sysconf(_SC_NPROCESSORS_ONLN);
```

## 9. Conclusiones

Esta implementación de multiplicación de matrices con hilos demuestra varios conceptos fundamentales de sistemas operativos:

1. **Creación y gestión de hilos**: Uso de pthreads para crear y sincronizar unidades de ejecución concurrentes.
2. **Compartición de recursos**: Cómo los hilos comparten el espacio de direcciones mientras mantienen sus propios contextos.
3. **Balanceo de carga**: Distribución equitativa del trabajo entre los hilos para maximizar la utilización de recursos.
4. **Sincronización**: Uso de pthread_join para asegurar que todos los cálculos se completen antes de continuar.
5. **Medición de rendimiento**: Evaluación de la efectividad de una implementación concurrente mediante métricas como speedup y eficiencia.


