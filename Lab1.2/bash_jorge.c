#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>

void parse(char* ins, char* args[], size_t size){
	int idx = 0;
	if(ins[size-1] == '\n')
		ins[size-1] = '\0';
	char *p = strtok(ins, " ");
	while(p != NULL){
		args[idx++] = p;
		p = strtok(NULL, " ");
	}
	args[idx] = 0;
}
		
int ls(char* args[]){
	DIR *dp;
	struct dirent *sd; 
	char pointer[1000];
	getcwd(pointer, sizeof pointer);
	dp = opendir((const char*)pointer);
	while((sd=readdir(dp))!=NULL){
		if(sd->d_name[0] == '.') continue;
		printf("%s\t",sd->d_name);
	}
	printf("\n");
	closedir(dp);
	return 0;
}

int pwd(char* args[]){
	char cwd[1000];
	getcwd(cwd, sizeof cwd);
	printf("%s\n",cwd);
	return 0;
}

int rm(char* args[]){
	remove(args[1]);
	return 0;
}

int cd(char* args[]){
	chdir(args[1]);
	return 0;
}

int mdir(char* args[]){
	mkdir(args[1], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	return 0;
}

int nano(char* args[]){
	execvp("nano", args);
	return 0;
}

int cat(char* args[]){
	execvp("cat", args);
	printf("\n");
	return 0;
}

int main(int argc, char *argv[]){
	char instruction[1000];
	printf("SOJorge $ ");
	while(fgets(instruction, 1000, stdin)){
		char *args[1000];
		if(strcmp(instruction, "\n") == 0) continue;
		parse(instruction, args, strlen(instruction));
		if(strcmp(args[0], "exit") == 0)
			break;
		else if(strcmp(args[0], "cd") == 0)
			cd(args);
		else{
			if(fork() == 0){
				if(strcmp(args[0], "ls") == 0)
					ls(args);
				else if(strcmp(args[0], "clear") == 0)
					printf("\033[H\033[J");
				else if(strcmp(args[0], "rm") == 0)
					rm(args);
				else if(strcmp(args[0], "pwd") == 0)
					pwd(args);
				else if(strcmp(args[0], "mkdir") == 0)
					mdir(args);
				else if(strcmp(args[0], "nano") == 0)
					nano(args);
				else if(strcmp(args[0], "cat") == 0)
					cat(args);
				else
					printf("%s is not a valid command\n", args[0]);
				return 0;
			}
			else wait(NULL);
		}
		printf("SOJorge $ ");
	}
	return 0;
}
