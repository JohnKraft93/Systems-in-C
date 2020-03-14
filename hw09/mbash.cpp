#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

int main() {	
	while(true){
		char buf[256];
		char *ptr;
		char **args = NULL;
		int size = 0;
		printf("MY_UNIX> ");			
		if(fgets(buf, 256, stdin) == NULL) {
			args = NULL;
		}
		else if(buf[0] == '\n') {
			args = (char**) realloc(args, sizeof(char*) * (size+1));
			if(args == NULL) {
				perror("REALLOC ERROR");
				exit(1);
			}
			args[0] = buf;

		} else {
			ptr = strchr(buf, '\n');
			*ptr = '\0';
			ptr = strtok(buf, " ");
			while(ptr != NULL) {
				args = (char**) realloc(args, sizeof(char*) * (size+1));	
				if(args == NULL) {
					perror("REALLOC ERROR");
					exit(1);
				}
				args[size] = strdup(ptr);
				if(args[size] == NULL) {
					perror("STRDUP ERROR");
					exit(1);
				}
				ptr = strtok(NULL, " ");	
				size++;
			}		
			args = (char**) realloc(args, sizeof(char*) * size+1);
			if(args == NULL) {
				perror("REALLOC ERROR");
				exit(1);
			}
			args[size] = NULL;
		}
		if(args == NULL) {
			printf("\n");
			exit(1);
		} else if (args[0][0] == '\n') {
			free(args);
			continue;
		} else if ( strcmp(args[0], "exit") == 0) {	
			int pos = 0;
			while(true) {
				if(args[pos] == NULL){
					break;
				} else {
					free(args[pos]);
				}
				pos++;
			}
			free(args);
			exit(1);
		} else {
			pid_t retval = fork();
			if(retval == 0){
				if(execvp(args[0], args)){
					printf("ERROR: could not exec\n");
					exit(1);
				}
			} else if(retval > 0){
				int status;
				if(wait(&status) == -1) {
					perror("WAIT ERROR");
					exit(1);
				}
				int pos = 0;
				while(true) {
					if(args[pos] == NULL){
						break;
					} else {
						free(args[pos]);
					}
					pos++;
				}
				free(args);	
			} else {
				perror("FORK ERROR");
				exit(1);
			}
		}
	}
}
