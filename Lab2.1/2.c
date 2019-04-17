#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

int main(){
	struct timeval start, end;
	struct timezone tz;
	long long totaltime = 0;
	for(int i = 0; i<100; i++){
		gettimeofday(&start, &tz);
		getpid();
		gettimeofday(&end, &tz);
		totaltime += end.tv_usec - start.tv_usec;
	}
	printf("%.3Lg\n", (long double)totaltime/(100000000.0));
	return 0;
}
