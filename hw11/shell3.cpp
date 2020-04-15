#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>


char **split(char *command, char **arg){
	int size = 0;
	char *ptr;
	ptr = strtok(command, "_");
	while(ptr != NULL){
		arg = (char**) realloc(arg, sizeof(char*) * (size+1));
		if(arg == NULL){
			perror("REALLOC ERROR");
			exit(1);
		}
		arg[size] = strdup(ptr);
		if(arg[size] == NULL){
			perror("STRDUP ERROR");
			exit(1);	
		}
		ptr = strtok(NULL, "_");
		size++;
	}
	arg = (char**) realloc(arg, sizeof(char*) * (size+1));
	if(arg == NULL){
		perror("REALLOC ERROR");
		exit(1);
	}
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
	if(arg != NULL){free(arg);}
}

void *closepipes(int com, int **pfds, int numpipes){
	for (int i = 0; i < numpipes; i++){
		if(i != com && i != com -1){
			for(int j = 0; j < 2; j++){
				if(close(pfds[i][j]) == -1){
					perror("CLOSE ERROR");
					exit(1);
				}
			}
		}
	}
}



int main(int argc, char **argv){
	if(argc < 3){
		printf("./shell must take two or more commands.\n");
		printf("./shell command_1 command_2 ...optional for more.\n");
		exit(1);
	}
	int **pfds = (int **) malloc(sizeof(int*) * argc-2);
	if(pfds == NULL){
		perror("MALLOC ERROR");
		exit(1);
	}
	int x;
	for(x = 0; x < argc-2; x++){
		pfds[x] = (int*) malloc(sizeof(int) * 2);
		if(pfds[x] == NULL){
			perror("MALLOC ERROR");
			exit(1);
		}
		pipe(pfds[x]);
	}
	pfds[x] = NULL;
	char **arg = NULL;

	for (int i = 0; i < argc-1; i++) {
		int retval = fork();
		if(retval == 0){
			closepipes(i, pfds, argc-2);
			//redirect stdin
			if(i != 0){
				close(pfds[i-1][1]);
				dup2(pfds[i-1][0], 0);
			}
			//redirect stdout
			if(i != argc-2){
				close(pfds[i][0]);		
				dup2(pfds[i][1], 1);
			}	
			arg = NULL;
			arg = split(argv[i+1], arg);
			if(execvp(arg[0],arg)){
				printf("EXEC ERROR\n");
				mfree(arg);
				exit(1);
			}
		} else if(retval < 0) {
			perror("FORK ERROR");
			exit(1);
		} 
	}

	for(int j = 0; j < argc-2; j++){
		if(close(pfds[j][0]) == -1){
			perror("CLOSE ERROR");
			exit(1);
		}

		if(close(pfds[j][1]) == -1){
			perror("CLOSE ERROR");
			exit(1);
		}
	}

	for(int i = 0; i < argc-1; i++){
		if(wait(NULL) == -1){
			perror("WAIT ERROR");
			exit(1);
		}
	}
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
