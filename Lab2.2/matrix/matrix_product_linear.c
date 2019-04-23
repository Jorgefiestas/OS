#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void opt_matrix(int **matrix, int rows, int columns){
	int * aux_matrix = malloc(sizeof(int)*rows*columns);
	for(int i = 0; i<rows; i++){
		matrix[i] = aux_matrix + i*columns;
	}
}

int main(){

	srand(time(NULL));

	//CACHE OPTIMIZED MATRICES

	int r1, c1;		//Number of rows and columns of matrix 1
	int r2, c2;		//Number of rows and columns of matrix 2

	printf("Inserte las dimensiones de la primera matriz (filas columnas): \n");
	scanf("%d %d", &r1, &c1);
	printf("Inserte las dimensiones de la segunda matriz (filas columnas): \n");
	scanf("%d %d", &r2, &c2);

	while(c1 != r2){
		printf("Por favor inserte matrices validas\n");
		printf("Inserte las dimensiones de la primera matriz (filas columnas): \n");
		scanf("%d %d", &r1, &c1);
		printf("Inserte las dimensiones de la segunda matriz (filas columnas): \n");
		scanf("%d %d", &r2, &c2);
	}

	int ** matrix1 = malloc(sizeof(int*)*r1);
	opt_matrix(matrix1, r1, c1);
	int ** matrix2 = malloc(sizeof(int*)*r2); 
	opt_matrix(matrix2, r2, c2);
	int ** matrix3 = malloc(sizeof(int*)*r1);
	opt_matrix(matrix3, r1, c2);

	for(int i = 0; i<r1; i++)
		for(int j = 0; j<c1; j++)
			matrix1[i][j] = rand()%6;

	for(int i = 0; i<r2; i++)
		for(int j = 0; j<c2; j++)
			matrix2[i][j] = rand()%6;


	struct timeval start;
	struct timeval end;

	gettimeofday(&start, NULL);

    for(int i = 0; i<r1; i++){
        for(int j = 0; j<c2; j++){
            matrix3[i][j] = 0;
            for(int k = 0; k<c1; k++){
                matrix3[i][j] += matrix1[i][k]*matrix2[k][j];
            }
        }
    }

	gettimeofday(&end, NULL);

	double dif = (double)(end.tv_sec-start.tv_sec)*1000000.0;
	dif += (double)(end.tv_usec-start.tv_usec);

	printf("%lf\n",dif/1000000.0);

	free(matrix1);
	free(matrix2);
	free(matrix3);

	return 0;
}
