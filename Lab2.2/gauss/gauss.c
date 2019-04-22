#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

#define NSIZE       8
#define VERIFY      1
#define t           4

#define SWAP(a,b)       {double tmp; tmp = a; a = b; b = tmp;}
#define SWAPINT(a,b)       {register int tmp; tmp = a; a = b; b = tmp;}
#define ABS(a)          (((a) > 0) ? (a) : -(a))

double **matrix,*B,*V,*C;
int *swap;
int *random1;

struct packed_data{
    int nsize;
    int i;
    int sp;
};

/* Allocate the needed arrays */

void allocate_memory(int size)
{
	double *tmp;
	int i;

	matrix = (double**)malloc(size*sizeof(double*));
	assert(matrix != NULL);
	tmp = (double*)malloc(size*size*sizeof(double));
	assert(tmp != NULL);

	for(i = 0; i < size; i++){
		matrix[i] = tmp;
		tmp = tmp + size;
	}

	B = (double*)malloc(size * sizeof(double));
	assert(B != NULL);
	V = (double*)malloc(size * sizeof(double));
	assert(V != NULL);
	C = (double*)malloc(size * sizeof(double));
	assert(C != NULL);
	swap = (int*)malloc(size * sizeof(int));
	assert(swap != NULL);
	random1= (int*)malloc(size *sizeof(int));
	assert(random1 !=NULL);
}

/* Initialize the matirx with some values that we know yield a
 * solution that is easy to verify. A correct solution should yield
 * -0.5, and 0.5 for the first and last C values consecutively, and 0
 * for the rest, though it should work regardless */

void initMatrix(int nsize)
{
	int i,j;
	for(i = 0 ; i < nsize ; i++){
		for(j = 0; j < nsize ; j++) {
			matrix[i][j] = ((j < i )? 2*(j+1) : 2*(i+1));
		}
		B[i] = (double)i;
		swap[i] = i;
	}
}

/* Get the pivot row. If the value in the current pivot position is 0,
 * try to swap with a non-zero row. If that is not possible bail
 * out. Otherwise, make sure the pivot value is 1.0, and return. */

void getPivot(int nsize, int currow)
{
	int i,irow;
	double big;
	double tmp;

	big = matrix[currow][currow];
	irow = currow;

	if (big == 0.0) {
		for(i = currow ; i < nsize; i++){
			tmp = matrix[i][currow];
			if (tmp != 0.0){
				big = tmp;
				irow = i;
				break;
			}
		}
	}

	if (big == 0.0){
		printf("The matrix is singular\n");
		exit(-1);
	}

	if (irow != currow){
		for(i = currow; i < nsize ; i++){
			SWAP(matrix[irow][i],matrix[currow][i]);
		}
		SWAP(B[irow],B[currow]);
		SWAPINT(swap[irow],swap[currow]);
	}


	{
		double pivotVal;
		pivotVal = matrix[currow][currow];

		if (pivotVal != 1.0){
			matrix[currow][currow] = 1.0;
			for(i = currow + 1; i < nsize; i++){
				matrix[currow][i] /= pivotVal;
			}
			B[currow] /= pivotVal;
		}
	}
}


/* For all the rows, get the pivot and eliminate all rows and columns
 * for that particular pivot row. */

void parallel(void* p){
    struct packed_data *pack = (struct packed_data *) p;
    int sp = pack->sp;
    int i = pack->i;
    int nsize = pack->nsize;

    for(int j = sp; j<nsize; j += t){
        double pivotVal = matrix[j][i];
        matrix[j][i] = 0.0;
        for(int k = i + 1 ; k < nsize; k++){
            matrix[j][k] -= pivotVal * matrix[i][k];
        }
        B[j] -= pivotVal * B[i];
    }
}

void computeGauss(int nsize){
	int i,j;
	int zz,xx;

	printf("Computing and the t is %d \n",*random1);

	for(i = 0; i < nsize; i++){
		getPivot(nsize,i);
		
		printf("\n\n ****After getting pivot and beginning interation number: %d \n",i);
		for (zz=0;zz<nsize;zz++){
			for(xx=0;xx<nsize;xx++){
                printf(" %f ",matrix[zz][xx]);
			}
			printf("\n");
		}
		
        int rc;
        pthread_t threads[t];
        struct packed_data pack[t];

		for (j = 0; j < t; j++){
            pack[j].nsize = nsize;
            pack[j].i = i;
            pack[j].sp = i + 1 + j;
            rc = pthread_create(threads + j, NULL, parallel,(void *)(pack+j));
            if (rc){
                printf("ERROR, codigo %d \n",rc);
                exit(-1);
            }

		}

        for (j = 0; j<t; j++){
            rc = pthread_join(threads[j],NULL);
            if (rc){
                printf("ERROR, codigo %d \n",rc);
                exit(-1);
            }
        }

		printf(" ****After you are done with  interation number: %d \n",i);
		for (zz=0;zz<nsize;zz++){
			for(xx=0;xx<nsize;xx++){
				printf(" %f ",matrix[zz][xx]);
			}
			printf("\n");
		}	
	
	}
}

/* Solve the equation. That is for a given A*B = C type of equation,
 * find the values corresponding to the B vector, when B, is all 1's */

void solveGauss(int nsize)
{
	int i,j;

	V[nsize-1] = B[nsize-1];
	for (i = nsize - 2; i >= 0; i --){
		V[i] = B[i];
		for (j = nsize - 1; j > i ; j--){
			V[i] -= matrix[i][j] * V[j];
		}
	}

	for(i = 0; i < nsize; i++){
		C[i] = V[i];//V[swap[i]];
	}
}

extern char * optarg;
int main(int argc,char *argv[])
{
	int i;
	struct timeval start;
	struct timeval finish;
	long compTime;
	double Time;
	int nsize = NSIZE;
	int var1;

    srand(time(NULL));

	while((i = getopt(argc,argv,"s:t:")) != -1){
		switch(i){
			case 's':
				{
					int s;
					s = atoi(optarg);
					if (s > 0){
						nsize = s;
					} else {
						fprintf(stderr,"Entered size is negative, hence using the default (%d)\n",(int)NSIZE);
					}
				}
				break;
			case 't' :
				var1 = atoi(optarg);
				break;			

			default:
				assert(0);
				break;
		}
	}

	allocate_memory(nsize);
	*random1	= var1;

	gettimeofday(&start, 0);
	printf("The value input is %d \n",var1);
	printf("Program begins with Size %d \n",nsize);
	initMatrix(nsize);
	computeGauss(nsize);
#if VERIFY
	solveGauss(nsize);
#endif
	gettimeofday(&finish, 0);

	compTime = (finish.tv_sec - start.tv_sec) * 1000000;
	compTime = compTime + (finish.tv_usec - start.tv_usec);
	Time = (double)compTime;

	printf("Application time: %f Secs\n",(double)Time/1000000.0);

#if VERIFY
	for(i = 0; i < nsize; i++)
		printf("%6.5f %5.5f\n",B[i],C[i]);
#endif

	return 0;
}
