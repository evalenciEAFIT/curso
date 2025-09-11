
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void multiplicar_matrices(int filas_A, int columnas_A, int **matrizA,
                          int filas_B, int columnas_B, int **matrizB,
                          int **matrizResultado, long long *operaciones);

void inicializar_matriz (int filas, int columnas, int **matriz);
void verMatriz(int filas, int columnas, int **matriz);


int main(int argc, char *argv[]){
   int filas_A = atoi(argv[1]);
   int columnas_A = atoi(argv[2]);
   int filas_B = atoi(argv[3]);
   int columnas_B = atoi(argv[4]);

   //inicio las variables memoria dinaca
   int **matriz_A =(int **)malloc(filas_A * sizeof(int *));
   for (int i=0; i<filas_A; i++) 
      matriz_A[i] = (int *)malloc(columnas_A * (sizeof(int)));
   
   int **matriz_B =(int **)malloc(filas_B * sizeof(int *));
   for (int i=0; i<filas_B; i++) 
      matriz_B[i] = (int *)malloc(columnas_B * (sizeof(int)));

   int **matrizResultado =(int **)malloc(filas_A * sizeof(int *));
   for (int i=0; i<filas_A; i++) 
      matrizResultado[i] = (int *)malloc(columnas_B * (sizeof(int)));


   //silla de laleatorio
   srand(time(NULL));

   printf("FilasA: %i Columnas_A: %i \n",filas_A,columnas_A);
   printf("FilasB: %i Columnas_B: %i \n",filas_B,columnas_A);
   
   inicializar_matriz(filas_A, columnas_A, matriz_A);
   inicializar_matriz(filas_B, columnas_B, matriz_B);

   printf("MATRIZ A....\n");
   verMatriz(filas_A, columnas_A, matriz_A);

   printf("MATRIZ B....\n");
   verMatriz(filas_B, columnas_B, matriz_B);

   long long operaciones = 0;
   //multiplicar
   multiplicar_matrices(filas_A, columnas_A, matriz_A,
                        filas_B, columnas_B, matriz_B,
                        matrizResultado, &operaciones);
   printf("Multiplicar MATRIZ....\n");
   verMatriz(filas_A, columnas_B, matrizResultado);
   printf("operaciones totales: %d",operaciones);
   

   //Limpiar la memoria virtual
   for (int i=0; i<filas_A; i++) free(matriz_A[i]); //liberar las columnas
   free(matriz_A);                                  //liberar las filas
   for (int i=0; i<filas_B; i++) free(matriz_B[i]);
   free(matriz_B);
   for (int i=0; i<filas_A; i++) free(matrizResultado[i]);
   free(matrizResultado);

   return 0;
   
}

void multiplicar_matrices(int filas_A, int columnas_A, int **matrizA,
                          int filas_B, int columnas_B, int **matrizB,
                          int **matrizResultado, long long *operaciones){
     for(int i=0; i< filas_A;i++){
        for(int j=0;j<columnas_B; j++){
           matrizResultado[i][j] = 0;
           for(int k=0;k<columnas_A;k++){
            matrizResultado[i][j] += matrizA[i][k] * matrizB[k][j];
            (*operaciones)+=2;
           }
        
        }
     }
}

void inicializar_matriz (int filas, int columnas, int **matriz){
   for(int i=0; i<filas; i++){
      for(int j=0; j<columnas; j++){
         matriz[i][j]=rand() % 10;
      }
   }
}

void verMatriz(int filas, int columnas, int **matriz){
   for(int i=0; i<filas; i++){
      for(int j=0; j<columnas; j++){
         printf("%d\t",matriz[i][j]);
      }
      printf("\n");
   }
}
