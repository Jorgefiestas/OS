#include <stdio.h>

int main(){
	int p[2], pid;
	char a;

	if(pipe(p)<0)
		exit(1);

	if(fork() == 0){
		read(p[0], a, 1); 
		write(p[1], 'a', 1);	
	}
	else{
		write(p[1], 'a', 1);
		read(p[0], a, 1); 
	}

	return 0;
}
