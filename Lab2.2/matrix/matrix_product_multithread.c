#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

struct packed_data{
	int **m1;
	int **m2;
	int **m3;
	int sp;
	int r;
	int r3;
	int c3;
	int t;
};

void multiply(void* p){

	struct packed_data *pack = (struct packed_data *) p;
	int **m1 = pack -> m1;	//matrix1
	int **m2 = pack -> m2;	//matrix2
	int **m3 = pack -> m3;	//matrix3
	int sp = pack -> sp;	//starting point
	int r  = pack -> r;		//number of rows of matrix 2
	int r3 = pack -> r3;	//number of rows of matrix 3
	int c3 = pack -> c3;	//number of columns of matrix 3
	int t  = pack -> t;		//number of threads

	for(int i = sp; i<r3*c3; i = i + t){
		m3[i/c3][i%c3] = 0;
		
		for(int j = 0; j<r; j++)
			m3[i/c3][i%c3] += m1[i/c3][j]*m2[j][i%c3];
	}
}

void opt_matrix(int **matrix, int rows, int columns){
	int * aux_matrix = malloc(sizeof(int)*rows*columns);
	for(int i = 0; i<rows; i++){
		matrix[i] = aux_matrix + i*columns;
	}
}

int main(){

	srand(time(NULL));

	//CACHE OPTIMIZED MATRICES

	int t;			//Number of threads
	int r1, c1;		//Number of rows and columns of matrix 1
	int r2, c2;		//Number of rows and columns of matrix 2

	printf("Inserte el numero de hilos a utilizar: \n");
	scanf("%d", &t);
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


	//MULTITHREAD DEFINITION

	int rc;
	pthread_t threads[t];
	struct packed_data pack[t];

	gettimeofday(&start, NULL);

	for(int i = 0; i<t; i++){
		pack[i].m1 = matrix1;
		pack[i].m2 = matrix2;
		pack[i].m3 = matrix3;
		pack[i].sp = i;
		pack[i].r = r2;
		pack[i].r3 = r1;
		pack[i].c3 = c2;
		pack[i].t = t;
		
		rc = pthread_create(threads + i, NULL, multiply,(void *)&pack[i]);
		
		if (rc){
			printf("ERROR, codigo %d \n",rc);
 			exit(-1);
 		}
	}

	for (int i=0; i<t; i++){
		rc = pthread_join(threads[i],NULL);
		if (rc){
			printf("ERROR, codigo %d \n",rc);
		 	exit(-1);
		}
	}

	gettimeofday(&end, NULL);

	double dif = (double)(end.tv_usec-start.tv_usec)/1000000.0;

	printf("%lf\n",dif);
	
	free(matrix1);
	free(matrix2);
	free(matrix3);

	return 0;
}
