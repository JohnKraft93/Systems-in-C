#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	bool files[3] = {false,false,false};
	char app = 'w';
	FILE *filesptr[3];
	char *pch;
	char *buf;
	char *filename;

	if(argc > 5){
		printf("./shell OPTIONS: (in=input.txt) (out=output.txt) (err=err.txt) (-append)\n"); 
		exit(1);
	}	
	for(int i = 1; i < argc; i++){
		//test for append command
		pch = strstr(argv[i], "-append");
		if(pch != NULL){
			app = 'a';
		}
	}
	
	for(int i = 1; i < argc; i++){
		//test for input file
		buf = argv[i];	
		pch = strstr(buf, "in=");
		if(pch != NULL){	
			filename = &buf[3];
			filesptr[0] = fopen(filename, "r");
			if(filesptr[0] == NULL){
				perror("INFILE ERROR");
				exit(1);
			}
			int fd = fileno(filesptr[0]);
			if(fd == -1){
				perror("INFILE FD ERROR");
				exit(1);
			}
			dup2(fd, 0);
			files[0] = true;	
		}				
		//test for output file
		pch = strstr(buf, "out=");
		if(pch != NULL){
			filename = &buf[4];	
			filesptr[1] = fopen(filename, &app);
			if(filesptr[1] == NULL){
				perror("OUTFILE ERROR");
				exit(1);
			}
			int fd = fileno(filesptr[1]);	
			if(fd == -1){
				perror("INFILE FD ERROR");
				exit(1);
			}
			dup2(fd,1);
			files[1] = true;
		}
		//test for error file
		pch = strstr(buf, "err=");
		if(pch != NULL){
			filename = &buf[4];	
			filesptr[2] = fopen(filename, &app);
			if(filesptr[2] == NULL){
				perror("ERRFILE ERROR");
				exit(1);
			}
			int fd = fileno(filesptr[2]);	
			if(fd == -1){
				perror("INFILE FD ERROR");
				exit(1);
			}
			dup2(fd, 2);
			files[2] = true;
		}
	}
	char line[80];
	while(fgets(line, 80, stdin)){
		fprintf(stdout, "stdout: %s", line);
		fprintf(stderr, "stderr: %s", line);
	}

	for(int i = 0; i < 3; i++){
		if(files[i] == true){			
			if(fclose(filesptr[i]) == -1){
				perror("FILE CLOSE ERROR");
			}
		}
	}
}	

