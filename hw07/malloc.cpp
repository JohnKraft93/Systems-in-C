#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

struct node{
	size_t size;
	node *prev;
	node *next;
};

int heapStart;
node *head;
node *tail;

void *mmalloc(size_t size){
	int PAGE = 1024;
	if(size > 1012) {
		printf("WARNING..CAN'T ALLOCATE MORE THAN 1012 BYTES.\n");
		return NULL;
	}
	if(size < 0) {
		printf("WARNING..CAN'T ALLOCATE LESS THAN 0 BYTES.\n");
		return NULL;
	}
	else {
		printf("mmalloc: requesting %d bytes\n", size);
		node *runner = head;
		while(true) {
			while(runner != NULL){
				if(size == runner->size || runner->size - size < 13) {
					if(runner->size >= size) {
						runner->size = size;
						if(runner->prev != NULL){
							runner->prev->next = runner->next;
							tail = runner->prev;
						}
						else {
							head = runner->next;
						}
						if(runner->next != NULL) {
							runner->next->prev = runner->prev;
						}
						else {
							tail = runner->prev;
						}
						printf("mmalloc: node at %d has enough space (%d)\n", (int) runner - heapStart, runner->size);
						printf("mmalloc: releasing %d (size=%d) from free list\n", (int) runner - heapStart, runner->size);
						printf("mmalloc: returning %d\n", (int)runner+12-heapStart);
						return (void*) (runner+1);
					}
				}
				if(size < runner->size) {
					int pos = (int) runner - heapStart;
					node *remain = (node*) ((int)runner + 12 + size);
					remain->size = (int) runner->size - size - 12;	
					if(runner->prev != NULL){
						runner->prev->next = remain;					
					}
					else {
						head = remain;
					}
					
					if(runner->next != NULL) {
						runner->next->prev = remain;				
					}
					else{ tail = remain;}
					remain->next = runner->next;
					remain->prev = runner->prev;	
					int posRe = (int) remain - heapStart;
					printf("mmalloc: node at %d has enough space (%d)\n", pos, runner->size);					
					printf("mmalloc: splitting %d (%d) into %d (%d) and %d (%d)\n", pos, runner->size, pos, size, posRe, remain->size);	
					runner->size = size;
					return (void*) (runner+1);
				}
				runner = runner->next;
			}
			printf("mmalloc: calling sbrk..\n");
			node *current;
			current = (node*) sbrk(1024);
			if (current == (void*) -1) {
				perror("SBRK ERROR");
				return NULL;
			}
			current->size = 1024 - 12;
			if (tail == NULL) {
				head = current;
				tail = current;
				current->prev = NULL;	
			}
			else {
				current->prev = tail;
				tail->next = current;
				tail = current;
			}
			current->next = NULL;
			runner = current;
		}
	}			
}

void *mfree(void *ptr){
	node* n =(node*) ptr - 1;	
	printf("mfree: node %d (size %d)\n", (int) n-heapStart, n->size);	
	if(tail == NULL) {
		head = n;
		tail = n;
		n->prev = NULL;
	}
	else {
		n->prev = tail;
		tail->next = n;
		tail = n;
		tail->next = NULL;
	}
}

void printFreeList(){
	printf("=== FREE LIST ===\n");
	node* runner = head;
	while(runner != NULL){
		printf("%d: (%d, %d, %d)\n",(int) runner - heapStart, 
			(int) runner->prev-heapStart >= 0 ? (int) runner->prev-heapStart:-1,
			(int) runner->next-heapStart >= 0 ? (int) runner->next-heapStart:-1, runner->size);
		runner = runner->next;
	}	
	printf("=== END       ===\n"); 
}

int main(int argc, char **argv) {
	printf("main: starting main()\n");
	heapStart = (int) sbrk(0);
	if(heapStart == -1) {
		perror("SBRK ERROR");
		exit(1);
	}
	head = NULL;
	tail = NULL;
	printFreeList();
	printf("main: testing imperfect match..\n");
	void *fp = mmalloc(10);
	printFreeList();

	printf("main: testing perfect match..\n");
	void *fp2 = mmalloc(1012);
	printFreeList();
	
	printf("main: free perfect match..\n");
	mfree(fp2);
	printFreeList();
	
	printf("main: testing imperfect match..\n");
	void *fp3 = mmalloc(50);
	printFreeList();


/*
	printf("main: testing perfect match..\n");
	void *fp = mmalloc(1012);
	printFreeList();

	printf("main: free perfect match..\n");
	mfree(fp);	
	printFreeList();

	printf("main: testing perfect match..\n");
	void *fp2 = mmalloc(1012);
	printFreeList();	

	printf("main: testing imperfect match..\n");
	void *fp3 = mmalloc(800);
	void *fp4 = mmalloc(750);	
	void *fp5 = mmalloc(700);
	void *fp6 = mmalloc(650);
	printFreeList();	

	printf("main: free imperfect match..\n");
	mfree(fp4);
	printFreeList();	
	
	printf("main: Remove middle node from free list..\n");
	void *fp7 = mmalloc(250);
	printFreeList();	
	
	printf("main: Remove start node from free list..\n");
	void *fp8 = mmalloc(200);
	printFreeList();		

	printf("main: Remove end node from free list..\n");
	void *fp9 = mmalloc(750);
	printFreeList();	
	
*/
}
