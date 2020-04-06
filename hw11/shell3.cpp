#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>


char **split(char *command, char **arg){
	int size = 0;
	char *ptr;
	*ptr = '\0';
	ptr = strtok(command, "_");
	while(ptr != NULL){
		arg = (char**) realloc(arg, sizeof(char*) * (size+1));
		arg[size] = strdup(ptr);
		ptr = strtok(NULL, "_");
		size++;
	}
	arg = (char**) realloc(arg, sizeof(char*) * (size+1));
	arg[size] = NULL;
	return arg;
}

void *mfree(char **arg){
	int pos = 0;
	while(true){
		if(arg[pos] == NULL){
			break;
		} else {
			free(arg[pos]);
		}
		pos++;
	}
	free(arg);
}

int main(int argc, char **argv){
	if(argc < 3){
		printf("./shell must take two or more commands.\n");
		printf("./shell command_1 command_2 ...optional for more.\n");
		exit(1);
	}

	int **pfds = (int **) malloc(sizeof(int*) * argc-2);
	int j;
	for(j = 0; j < argc-2; j++){
		pfds[j] = (int*) malloc(sizeof(int) * 2);
		pipe(pfds[j]);
	}
	pfds[j] = NULL;
//	int pfd[2];
//	pipe(pfd);
	char **arg;

	//command 1
	int retval = fork();
	if(retval == 0){
		close(pfds[0][0]);
		dup2(pfds[0][1], 1);
		arg = NULL;
		arg = split(argv[1], arg);
		execvp(arg[0],arg);
		mfree(arg);
	} else if(retval < 0){
		perror("FORK ERROR");
		exit(1);
	}
	//command 2
	retval = fork();
	if(retval == 0){
		close(pfds[0][1]);
		dup2(pfds[0][0], 0);
		arg = NULL;
		arg = split(argv[2], arg);
		execvp(arg[0], arg);
		mfree(arg);
	} else if(retval < 0){
		perror("FORK ERROR");
		exit(1);
	}

	close(pfds[0][0]);
	close(pfds[0][1]);
	wait(NULL);
	wait(NULL);
	int pos = 0;
	while(true){
		if(pfds[pos] == NULL){
			break;
		} else {
			free(pfds[pos]);
		}
		pos++;
	}
	free(pfds);
}
