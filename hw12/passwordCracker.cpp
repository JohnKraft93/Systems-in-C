#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

struct Password {
	char *first;
	char *second;
//	char *guessFirst;
//	char *guessSecond;
};

void *run(void *v){
	struct Password p = (struct Password) v;
	printf("f =%s   s =%s\n", p.first, p.second);
}


int main(int argc, char **argv){
	if(argc != 4){
		printf("usage: ./PC words.txt seed N\n");
		exit(1);
	}

	if(argv[2] < 0){
		printf("Number of threads must be > 0");
		exit(1);
	}

	char word[256];
	char **words = NULL;
	FILE *fp = fopen(argv[1], "r");
	if(fp == NULL){
		perror("FILE READ ERROR");
		exit(1);
	}
	
	int size = 0;
	while(fgets(word, 256, fp) != NULL){
		//printf("%s\n", word);

		char *ptr = strchr(word, '\n');
		*ptr = '\0';
		words = (char**) realloc(words, sizeof(char*) * (size+1));
		if(words == NULL){
			perror("REALLOC ERROR");
			exit(1);
		}
		words[size] = strdup(word);
		if(words[size] == NULL){
			perror("STRDUP ERROR");
			exit(1);
		}
		size++;
	}
	
	words = (char**) realloc(words, sizeof(char*) * (size+1));
	if(words == NULL){
		perror("REALLOC ERROR");
		exit(1);
	}
	words[size] = NULL;
	fclose(fp);
	int i = 0;
	while(words[i] != NULL){
//		printf("%s\n", words[i]);
		i++;
	}
	struct Password password;
	srand(atoi(argv[1]));
//	printf("HEE\n");
	int x = rand();
	int y = rand() % size;
//	printf("rand() = %d\n", x);
//	printf("size = %d\n", size);
//	printf("r mod s = %d\n", y);

	char *p = words[4];
//	printf("%s\n", p);
	password.first = words[rand() % size];
	password.second = words[rand() % size];
	
//	printf("HEE\n");
	printf("%s %s \n", password.first, password.second);
	
	printf("HEE\n");
	pthread_t threads[atoi(argv[2])];
	div_t ret = div(size, atoi(argv[2]));
	printf("%d \n", ret.quot);
	printf("%d \n", ret.rem);

	for(int j = 0; j < ret.quot + 1; j++){
		printf("HELLO\n");
		for(long i = 0; i < atoi(argv[2]); i++){		
			printf("r d\n");
		//	password->guessfirst = words[i];
		//	password->guessSecond = words[j];
			pthread_create(&threads[i], NULL, run,(void*) password);
		}

		for(int i = 0; i < atoi(argv[2]); i++){
			pthread_join(threads[i], NULL);
		}
	}

}
