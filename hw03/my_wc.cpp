#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <stdlib.h>
using namespace std;

int main(int argc, char **argv){	
	if(argc == 1) {
		fprintf(stderr, "./my_wc file [ file ... ]\n");
		exit(1);
	}
	else{
		int total_lines = 0;	
        	int total_chars = 0;
        	int total_words = 0;	
		char *ptr;
                char line[80];
                vector<char *> lines;
		vector<char *> words;	
		int chars = 0;
		for(int numoffiles = 0; numoffiles < argc-1; numoffiles++){
			lines.clear();
			words.clear();
			chars = 0;
			FILE *fp = fopen(argv[numoffiles+1], "r");
			if(fp == NULL) {
				perror("File won't open");
				exit(1);
			}	
			while(fgets(line, 80, fp)) {	
				ptr = strchr(line, '\n');
				*ptr = '\0';
				lines.push_back(strdup(line));
			}	
			for (int i = lines.size() - 1; i >=0; i--){
				ptr = strtok(lines[i], " ");
				while(ptr != NULL){
					words.push_back(strdup(ptr));				
					ptr = strtok(NULL, " ");
				}
			}
			for (int j = words.size() - 1; j >= 0; j--) {
				chars += strlen(words[j]);			
			}
			printf("LINES: %d WORDS: %d CHARS: %d %s\n", lines.size(), words.size(), chars, argv[numoffiles+1]);
			total_lines += lines.size();
			total_words += words.size();
			total_chars += chars;
			fclose(fp);
		}
		if(argc > 2){printf("LINES: %d WORDS: %d CHARS: %d TOTAL\n", total_lines, total_words, total_chars);}	
	}
}
