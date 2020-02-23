#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <utime.h>

int main(int argc, char **argv) {
	if(argc < 3) {
		fprintf(stderr, "usage: ./mtar -a file.mtar file1 [file2]\n");
		fprintf(stderr, "usage: ./mtar -x file.mtar\n");
		exit(1);
	}
	if(strcmp(argv[1], "-a") == 0){
		struct stat sb;
		FILE *fpa = fopen(argv[2], "w");
		if(fpa != NULL){
			for(int i = 3; i < argc; i++){
				FILE *fp = fopen(argv[i], "r");
				if(fp != NULL){
					int st = stat(argv[i], &sb);
					if(st == -1) {
						perror("Stat Buffer Error");
						exit(1);
					}
					if(S_ISREG(sb.st_mode)){ 
						fprintf(stdout, "archiving %s (size %lu)\n", argv[i], sb.st_size);
						fprintf(fpa,"%s\n",argv[i]);
						int fw = fwrite(&sb, sizeof(struct stat), 1, fpa);	
						if(fw != 1) {
							perror("FILE WRITE ERROR");
							exit(1);
						}
						fprintf(fpa, "\n");
						char d;
						while(true){
							int r = fread(&d, 1, 1, fp);
							if(r == 0){break;}
							else if(r != 1){
								perror("FILE READ ERROR");
								exit(1);
							}
							int w = fwrite(&d, 1, 1, fpa);
							if (w != 1) {		
								perror("FILE WRITE ERROR");
								exit(1);
							}
						}				
						int fc1 = fclose(fp);
						if(fc1 == -1) {
							perror("FILE CLOSE ERROR");
							exit(1);
						}
					}
				}
				else{
					perror("FILE NOT FOUND");
				}	
			}	
			int fc2 = fclose(fpa);
			if(fc2 == -1) {
				perror("FILE CLOSE ERROR");
				exit(1);
			}	
		}
	}
	else if(strcmp(argv[1], "-x") == 0) {
		struct stat sb;
		struct utimbuf utb;
		FILE *fpr = fopen(argv[2], "r");
		if (fpr != NULL){
			char file[100];
			while(fscanf(fpr,"%s", file) != EOF){	
				FILE *fp = fopen(file, "w");
				if(fp != NULL) {
					int fs = fseek(fpr, 1, SEEK_CUR);
					if(fs != 0){
						perror("FILE SEEK ERROR");
						exit(1);
					}
					int rstat = fread(&sb, sizeof(struct stat), 1, fpr);
					if (rstat == -1) {
						perror("FILE STRUCT READ ERROR");
						exit(1);
					}
					fprintf(stdout, "extracting %s (size %lu)\n", file, sb.st_size);
					int c = chmod(file, sb.st_mode);
					if(c == -1) {
						perror("CHMOD ERROR");
						exit(1);
					}
					char *datasize = (char*) malloc(sizeof(char)*sb.st_size);
					if(datasize == NULL) {
						perror("MALLOC ERROR");
						exit(1);
					}
					fs = fseek(fpr, 1, SEEK_CUR);
					if(fs != 0){
						perror("FILE SEEK ERROR");
						exit(1);
					}
					int re = fread(datasize, sb.st_size, 1, fpr);	
					if(re != 1){
						perror("WRITE ERROR");
						exit(1);
					}	
					int wr = fwrite(datasize, sb.st_size, 1, fp);
					if(wr != 1){
						perror("WRITE ERROR");
						exit(1);
					}	
					int fc1 = fclose(fp);
					if(fc1 == -1) {
						perror("FILE CLOSE ERROR");
						exit(1);
					}
					free(datasize);	
					utb.actime = sb.st_atime;
					utb.modtime = sb.st_mtime;
					int ut = utime(file, &utb);
					if(ut == -1) {
						perror("UTIME ERROR");
						exit(1);
					}
				}
				else {
					perror("CAN'T EXTRACT");
				}
			}
		}		
		int fc2 = fclose(fpr);
		if(fc2 == -1) {
			perror("FILE CLOSE ERROR");
			exit(1);
		}
	}
	else{	
		fprintf(stderr, "usage: ./mtar -a file.mtar file1 [file2]\n");	
		fprintf(stderr, "usage: ./mtar -x file.mtar\n");
		exit(1);
	}	
}
