#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <utime.h>

int main(int argc, char **argv){
	if(argc < 8){
		fprintf(stderr, "you must enter 7 parameter.\n");
	}
	else {		
		struct tm fts;
		struct utimbuf utb;

		int a2 = atoi(argv[2]);
		if(a2 > 0 && a2 <=12) {
			fts.tm_mon = (a2-1);
		}
		else {
			fprintf(stderr, "Must enter a legit month.\n");
			exit(1);
		}
		
		int a3 = atoi(argv[3]);
		if(a3 > 0 && a3 <=31) {
			fts.tm_mday = a3;
		}
		else {
			fprintf(stderr, "Must enter a legit day of the month.\n");
			exit(1);
		}

		int a4 = atoi(argv[4]);
		if(a4 > 1900) {	
			fts.tm_year = a4-1900;
		}
		else {	
			fprintf(stderr, "Must enter a legit year.\n");
			exit(1);
		}

		int a5 = atoi(argv[5]);
		if(a5 >= 0 && a5 < 24) {
			fts.tm_hour = a5+1;
		}
		else {
			fprintf(stderr, "Must enter a legit hour.\n");
			exit(1);
		}

		int a6 = atoi(argv[6]);
		if(a6 >= 0 && a6 < 60) {
			fts.tm_min = a6;
		}
		else {
			fprintf(stderr, "Must enter a legit minute.\n");
			exit(1);
		}

		int a7 = atoi(argv[7]);
		if(a7 >= 0 && a7 < 60) {
			fts.tm_sec = a7;
		}
		else {
			fprintf(stderr, "Must enter a legit second.\n");
			exit(1);
		}

		time_t tim = mktime(&fts);
		if(tim == -1) {
			perror("MKTIME ERROR");
			exit(1);
		}

		char* wordTim = ctime(&tim);
		if(wordTim == NULL) {
			perror("CTIME ERROR");
			exit(1);
		}

		printf("Changing file %s timestamps to %s", argv[1], wordTim);
		utb.actime = tim;
		utb.modtime = tim;
		int ut = utime(argv[1], &utb);
		if(ut == -1) {
			perror("UTIME ERROR");
			exit(1);
		}
	}
}
