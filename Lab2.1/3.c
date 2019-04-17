#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

int main(){
	int p1[2], p2[2], pid;
	char a = 'a';

	if(pipe(p1)<0)
		exit(1);
	if(pipe(p2)<0)
		exit(1);

	struct timeval start, end;
	long inter = 0;
	long t = 0;

	for(int i = 0; i<20; i++){
		if(fork() == 0){
			gettimeofday(&start, 0);
			write(p2[1], &a, 1);	
			read(p1[0], &a, 1); 
			gettimeofday(&end, 0);
			read(p1[0], &a, 1);
			inter = (long)(end.tv_usec - start.tv_usec);
			write(p2[1], &inter, sizeof(long));
			return 0;
		}
		else{
			gettimeofday(&start, 0);
			write(p1[1], &a, 1);
			read(p2[0], &a, 1); 
			gettimeofday(&end, 0);
			write(p1[1], &a, 1);
			read(p2[0], &inter, sizeof(long)); 
		}
		t += (long)(end.tv_usec - start.tv_usec - inter);
	}
	printf("%.3Lg\n", (long double)t/40000000.0);
	return 0;
}
