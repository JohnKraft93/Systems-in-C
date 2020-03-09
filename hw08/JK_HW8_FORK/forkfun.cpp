#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

void mwrite(FILE *fp){
	char op[5] = {'+', '-' , '*', '/', '%'};
	fprintf(fp, "%d %c %d\n", rand() % 10, op[rand()%5], rand() % 10);

}

void mread(FILE *fp) {
		int firstval, secondval, results;
		char operation;
		while(fscanf(fp, "%d %c %d", &firstval, &operation, &secondval) != EOF){
			if((operation == '/' || operation == '%') && secondval == 0){
				results = -1;
			} else {
				switch(operation) {
					case '+':
						results = firstval + secondval;
						break;

					case '-':
						results = firstval - secondval;
						break;

					case '*':
						results = firstval * secondval;
						break;

					case '/':
						results = firstval / secondval;		
						break;

					case '%':
						results = firstval % secondval;
						break;
				}
			}
			printf("%d %c %d = %d\n", firstval, operation, secondval, results);
		}
}

int main(int argc, char **argv){	
	if(argc < 3 || argc > 3) {
		printf("./forkfun file.txt (number of expressions)\n");
		exit(1);
	} else{
		pid_t retval = fork();
		FILE *fp;
		if(retval == 0) {
			fp = fopen(argv[1], "w");
			if(fp == NULL){
				perror("child file error");
				exit(1);
			}	
			srand(time(NULL));
			for(int i = 0; i < atoi(argv[2]); i++){			
				mwrite(fp);
			}
			if(fclose(fp) == -1) {
				perror("child file close error");
				exit(1);
			}
		} else if(retval > 0) {
			int *status;
			pid_t wa = wait(status);
			if(wa == -1) {	
				perror("parent wait error");
				exit(1);
			}
			fp = fopen(argv[1], "r");
			if(fp == NULL) {
				perror("parent file error");
				exit(1);
			}
			mread(fp);
			if(fclose(fp) == -1) {
				perror("parent file exit error");
				exit(1);
			}
		} else {
			perror("fork fail");
			exit(1);
		}
	}
}
