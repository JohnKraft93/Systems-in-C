#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

int main(int argc, char **argv){ 
	char in[1];
	if (argc == 1) {
		while(scanf("%c", in)!= EOF){
			printf("%s", in);
		}
	}
	else {
		for (int len = 1; len <  argc; len++){
			FILE *fp = fopen(argv[len], "r");
			while(fscanf(fp,"%c", in)!= EOF) {
				printf("%s", in);
			}
			fclose(fp);
		}
	}
}


