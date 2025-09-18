### **Módulo 2: Concurrencia y Sincronización (Semanas 5-9)**
Este es el corazón de la concurrencia, uno de los temas más desafiantes y fundamentales.

---
Ejemplo con la multiplicación de matrices
# Concurrencia en Multiplicación de Matrices

## Código Base (Versión Secuencial)

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>

// Función para multiplicar dos matrices
void multiplicarMatriz(int filas_A, int columnas_A, int **matriz_A,
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
    multiplicarMatriz(filas_A, columnas_A, matriz_A,
                      filas_B, columnas_B, matriz_B,
                      matriz_resultado, &operaciones);
    fin = clock();
    tiempo_CPU_usado = ((double)(fin-inicio))/CLOCKS_PER_SEC;  // Corrección de cálculo

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

// Implementación de la multiplicación de matrices
void multiplicarMatriz(int filas_A, int columnas_A, int **matriz_A,
                       int filas_B, int columnas_B, int **matriz_B,
                       int **matriz_resultado, long long *operaciones){
    // Triple bucle anidado para multiplicación
    for(int i=0; i < filas_A; i++){              // Recorre filas de A
        for(int j=0; j < columnas_B; j++){       // Recorre columnas de B
            matriz_resultado[i][j] = 0;          // Inicializa elemento resultado
            for(int k=0; k < columnas_A; k++){   // Recorre columnas de A/filas de B
                // Multiplica elementos correspondientes y acumula
                matriz_resultado[i][j] += matriz_A[i][k] * matriz_B[k][j];
                (*operaciones) += 2;  // Contabiliza multiplicación y suma
            }
        }
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

## Explicación de la Multiplicación Secuencial

La multiplicación de matrices es una operación fundamental en computación con complejidad O(n³) para matrices cuadradas. El algoritmo implementado sigue el método clásico:

1. **Estructura de bucles anidados**:
   - Bucle exterior (`i`): Recorre cada fila de la matriz A
   - Bucle intermedio (`j`): Recorre cada columna de la matriz B
   - Bucle interior (`k`): Calcula el producto punto entre la fila i de A y la columna j de B

2. **Cálculo del resultado**:
   ```
   C[i][j] = Σ(A[i][k] * B[k][j]) para k desde 0 hasta columnas_A-1
   ```

3. **Características del algoritmo secuencial**:
   - Cada elemento de la matriz resultado se calcula independientemente
   - El cálculo sigue un orden estricto: fila por fila, elemento por elemento
   - No hay dependencias entre los cálculos de diferentes elementos de la matriz resultado

## Oportunidades para la Concurrencia

La multiplicación de matrices tiene características que la hacen ideal para la concurrencia:

### 1. Paralelización a nivel de filas

**Idea**: Asignar cada fila de la matriz resultado a un hilo diferente.
[Multiplicación de Matrices con Hilos](multiplicacionmatrices_hilos.md)
```c
// Estructura para pasar datos a los hilos
typedef struct {
    int fila_inicio;
    int fila_fin;
    int filas_A;
    int columnas_A;
    int columnas_B;
    int **matriz_A;
    int **matriz_B;
    int **matriz_resultado;
    long long *operaciones;
} ThreadData;

// Función que ejecutará cada hilo
void *procesarFilas(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    
    for(int i=data->fila_inicio; i<data->fila_fin; i++){
        for(int j=0; j<data->columnas_B; j++){
            data->matriz_resultado[i][j] = 0;
            for(int k=0; k<data->columnas_A; k++){
                data->matriz_resultado[i][j] += data->matriz_A[i][k] * data->matriz_B[k][j];
                (*(data->operaciones)) += 2;
            }
        }
    }
    
    pthread_exit(NULL);
}

// Función principal de multiplicación con hilos
void multiplicarMatrizConcurrente(int filas_A, int columnas_A, int **matriz_A,
                                  int filas_B, int columnas_B, int **matriz_B,
                                  int **matriz_resultado, long long *operaciones,
                                  int num_hilos) {
    
    pthread_t hilos[num_hilos];
    ThreadData datos_hilos[num_hilos];
    
    int filas_por_hilo = filas_A / num_hilos;
    int filas_extra = filas_A % num_hilos;
    
    // Crear hilos
    for(int i=0; i<num_hilos; i++) {
        datos_hilos[i].fila_inicio = i * filas_por_hilo;
        datos_hilos[i].fila_fin = (i+1) * filas_por_hilo;
        
        // Distribuir filas restantes
        if(i == num_hilos-1) {
            datos_hilos[i].fila_fin += filas_extra;
        }
        
        datos_hilos[i].filas_A = filas_A;
        datos_hilos[i].columnas_A = columnas_A;
        datos_hilos[i].columnas_B = columnas_B;
        datos_hilos[i].matriz_A = matriz_A;
        datos_hilos[i].matriz_B = matriz_B;
        datos_hilos[i].matriz_resultado = matriz_resultado;
        datos_hilos[i].operaciones = operaciones;
        
        pthread_create(&hilos[i], NULL, procesarFilas, &datos_hilos[i]);
    }
    
    // Esperar a que todos los hilos terminen
    for(int i=0; i<num_hilos; i++) {
        pthread_join(hilos[i], NULL);
    }
}
```

**Ventajas**:
- Implementación directa con pthreads
- Buena distribución de carga para matrices grandes
- Mínimas dependencias entre hilos

**Consideraciones**:
- Necesitar sincronización para el contador de operaciones
- Cada hilo necesita acceso a todas las matrices de entrada

### 2. Paralelización a nivel de bloques

**Idea**: Dividir las matrices en bloques más pequeños y procesar cada bloque en paralelo.

```c
// Función para procesar un bloque específico
void procesarBloque(int fila_inicio, int fila_fin, 
                    int col_inicio, int col_fin,
                    int filas_A, int columnas_A,
                    int **matriz_A, int **matriz_B,
                    int **matriz_resultado, long long *operaciones) {
    
    for(int i=fila_inicio; i<fila_fin; i++){
        for(int j=col_inicio; j<col_fin; j++){
            matriz_resultado[i][j] = 0;
            for(int k=0; k<columnas_A; k++){
                matriz_resultado[i][j] += matriz_A[i][k] * matriz_B[k][j];
                (*operaciones) += 2;
            }
        }
    }
}

// Función principal con procesamiento por bloques
void multiplicarPorBloques(int filas_A, int columnas_A, int **matriz_A,
                           int filas_B, int columnas_B, int **matriz_B,
                           int **matriz_resultado, long long *operaciones,
                           int tam_bloque) {
    
    // Procesar toda la matriz en bloques
    for(int ii=0; ii<filas_A; ii+=tam_bloque) {
        for(int jj=0; jj<columnas_B; jj+=tam_bloque) {
            // Calcular límites del bloque actual
            int fila_fin = (ii + tam_bloque > filas_A) ? filas_A : ii + tam_bloque;
            int col_fin = (jj + tam_bloque > columnas_B) ? columnas_B : jj + tam_bloque;
            
            // Aquí podríamos crear un hilo para procesar este bloque
            procesarBloque(ii, fila_fin, jj, col_fin,
                          filas_A, columnas_A,
                          matriz_A, matriz_B,
                          matriz_resultado, operaciones);
        }
    }
}
```

**Ventajas**:
- Mejor aprovechamiento de la caché del procesador
- Balance de carga más uniforme
- Escalabilidad superior para matrices muy grandes

**Consideraciones**:
- Requiere ajuste del tamaño de bloque según el hardware
- Implementación más compleja que la paralelización por filas

---

### 3. Paralelización anidada

**Idea**: Combinar paralelización a nivel de filas con paralelización dentro de cada fila.

```c
// Función que procesa una fila específica
void procesarFila(int fila, int columnas_A, int columnas_B,
                  int **matriz_A, int **matriz_B,
                  int **matriz_resultado, long long *operaciones) {
    
    for(int j=0; j<columnas_B; j++){
        matriz_resultado[fila][j] = 0;
        for(int k=0; k<columnas_A; k++){
            matriz_resultado[fila][j] += matriz_A[fila][k] * matriz_B[k][j];
            (*operaciones) += 2;
        }
    }
}

// Función principal con paralelización anidada
void multiplicarAnidado(int filas_A, int columnas_A, int **matriz_A,
                        int filas_B, int columnas_B, int **matriz_B,
                        int **matriz_resultado, long long *operaciones,
                        int num_hilos_externos, int num_hilos_internos) {
    
    // Aquí se crearían los hilos externos
    for(int i=0; i<filas_A; i++) {
        // Dentro de cada hilo externo, se crearían los hilos internos
        procesarFila(i, columnas_A, columnas_B,
                    matriz_A, matriz_B,
                    matriz_resultado, operaciones);
    }
}
```

**Ventajas**:
- Máximo paralelismo posible
- Buen rendimiento en sistemas con muchos núcleos

**Consideraciones**:
- Puede generar sobrecarga por creación excesiva de hilos
- Requiere ajuste del número de hilos según el hardware

---

## Implementación Recomendada   

[codigo](multiplicacionmatricas_hilos_procesos.md)

Para una implementación eficiente con concurrencia, recomendamos:

1. **Comenzar con paralelización a nivel de filas** por su simplicidad
2. **Optimizar el acceso a memoria**:
   - Asegurar que las matrices estén almacenadas por filas (row-major)
   - Considerar la transposición de la matriz B para mejorar localidad espacial
3. **Usar un número adecuado de hilos**:
   - Generalmente igual al número de núcleos del procesador
   - Ajustar según el tamaño de las matrices

```c
// Versión con pthreads (paralelización por filas)
#include <pthread.h>

typedef struct {
    int id_hilo;
    int filas_A;
    int columnas_A;
    int columnas_B;
    int **matriz_A;
    int **matriz_B;
    int **matriz_resultado;
    long long operaciones;
} DatosHilo;

void *procesarFilas(void *arg) {
    DatosHilo *datos = (DatosHilo *)arg;
    int filas_por_hilo = datos->filas_A / NUM_HILOS;
    int fila_inicio = datos->id_hilo * filas_por_hilo;
    int fila_fin = (datos->id_hilo == NUM_HILOS-1) ? datos->filas_A : fila_inicio + filas_por_hilo;
    
    for(int i=fila_inicio; i<fila_fin; i++){
        for(int j=0; j<datos->columnas_B; j++){
            datos->matriz_resultado[i][j] = 0;
            for(int k=0; k<datos->columnas_A; k++){
                datos->matriz_resultado[i][j] += datos->matriz_A[i][k] * datos->matriz_B[k][j];
                datos->operaciones += 2;
            }
        }
    }
    
    pthread_exit(NULL);
}

void multiplicarMatrizConcurrente(int filas_A, int columnas_A, int **matriz_A,
                                  int filas_B, int columnas_B, int **matriz_B,
                                  int **matriz_resultado, long long *operaciones) {
    
    pthread_t hilos[NUM_HILOS];
    DatosHilo datos[NUM_HILOS];
    long long operaciones_totales = 0;
    
    // Crear hilos
    for(int i=0; i<NUM_HILOS; i++) {
        datos[i].id_hilo = i;
        datos[i].filas_A = filas_A;
        datos[i].columnas_A = columnas_A;
        datos[i].columnas_B = columnas_B;
        datos[i].matriz_A = matriz_A;
        datos[i].matriz_B = matriz_B;
        datos[i].matriz_resultado = matriz_resultado;
        datos[i].operaciones = 0;
        
        pthread_create(&hilos[i], NULL, procesarFilas, &datos[i]);
    }
    
    // Esperar a que todos los hilos terminen
    for(int i=0; i<NUM_HILOS; i++) {
        pthread_join(hilos[i], NULL);
        operaciones_totales += datos[i].operaciones;
    }
    
    *operaciones = operaciones_totales;
}
```

## Conclusiones

La multiplicación de matrices es un excelente caso de estudio para la concurrencia debido a:

1. **Gran paralelismo inherente**: Cada elemento del resultado se calcula independientemente
2. **Alta densidad computacional**: Muchas operaciones por elemento de datos
3. **Patrones de acceso predecibles**: Facilita la optimización de memoria

La implementación de concurrencia puede reducir significativamente el tiempo de ejecución, especialmente para matrices grandes. La elección del enfoque de paralelización depende de:
- Tamaño de las matrices
- Características del hardware (número de núcleos, tamaño de caché)
- Requisitos de precisión y rendimiento

Para obtener el máximo rendimiento, es recomendable experimentar con diferentes estrategias de paralelización y ajustar los parámetros según el hardware específico.
```

