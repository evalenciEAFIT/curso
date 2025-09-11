#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void inicializar_matriz(int filas, int columnas, int **matriz);

void multiplicar_matrices ( int filas_A, int columnas_A, int **matriz_A,
                            int filas_B, int columnas_B, int **matriz_B,
                            int **matriz_resultado, long long *operaciones);

void imprimir_matriz(int filas, int columnas, int **matriz);

int main(int argc, char *argv[]){
    int filas_A =    atoi(argv[1]);
    int columnas_A = atoi(argv[2]);
    int filas_B =    atoi(argv[3]);
    int columnas_B = atoi(argv[4]);

    printf("Filas_A: %i  Columnas A: %i \n",filas_A, columnas_A);
    printf("Filas_B: %i  Columnas B: %i \n",filas_B, columnas_B);
    
    srand(time(NULL));
    int **matriz_A = (int **)malloc(filas_A * sizeof(int *));
    for (int i=0; i<filas_A; i++){
        matriz_A[i] = (int *)malloc(columnas_A * sizeof(int));
    }

    int **matriz_B = (int **)malloc(filas_B * sizeof(int *));
    for (int i=0; i<filas_B; i++){
        matriz_B[i] = (int *)malloc(columnas_B * sizeof(int));
    }

    int **matriz_resultado = (int **)malloc(filas_A * sizeof(int *));
    for (int i=0; i<filas_A; i++){
        matriz_resultado[i] = (int *)malloc(columnas_B * sizeof(int));
    }

    inicializar_matriz(filas_A, columnas_A, matriz_A);
    inicializar_matriz(filas_B, columnas_B, matriz_B);

    long long operadores=0;
    multiplicar_matrices(filas_A, columnas_A, matriz_A,   
                         filas_B, columnas_B, matriz_B,
                         matriz_resultado, &operadores);

    printf("Matrix A \n");
    imprimir_matriz(filas_A, columnas_A, matriz_A);

    printf("\n-------------------\n Matriz B \n");
    imprimir_matriz(filas_B, columnas_B, matriz_B);

    printf("\n-------------------\n Matriz Resutado \n");
    imprimir_matriz(filas_A, columnas_B, matriz_resultado);
   
    return 0;
}

void inicializar_matriz(int filas, int columnas, int **matriz){
    for (int i=0; i<filas; i++){
        for(int j=0; j<columnas;j++){
            matriz[i][j] = rand()%10;
        }
    }
}

void imprimir_matriz(int filas, int columnas, int **matriz){
    for (int i=0; i<filas; i++){
        for(int j=0; j<columnas;j++){
            printf("%d\t",matriz[i][j]);
        }
        printf("\n");
    }
}

void multiplicar_matrices ( int filas_A, int columnas_A, int **matriz_A,
                            int filas_B, int columnas_B, int **matriz_B,
                            int **matriz_resultado, long long *operaciones){
    for(int i=0; i<filas_A; i++){
        for(int j=0; j<columnas_B;j++){
            matriz_resultado[i][j] = 0;
            for(int k=0;k<columnas_A ;k++){
                matriz_resultado[i][j] += matriz_A[i][k] * matriz_B[k][j];
                (*operaciones)+=2;
            }

        }
    }
}
