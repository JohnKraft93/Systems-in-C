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
	else {
		printf("mmalloc requesting %d bytes\n", size);
		node *runner = head;
		while(true) {
			while(runner != NULL){
				if(size == runner->size || runner->size - size < 13) {
					if(runner->size >= size) {
						runner->size = size;
						if(runner->prev != NULL){
							runner->prev->next = runner->next;
						}
						if(runner->next != NULL) {
							runner->next->prev = runner->prev;
						}
						tail = runner->prev;
						printf("malloc: Node at %d has enough space %d\n", (int) runner - heapStart, runner->size);
						printf("Releasing %d (size=%d) from free list\n", (int) runner - heapStart, runner->size);
						printf("Returning %d\n", (int)runner+12-heapStart);
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
					printf("mmalloc: Node at %d has enough space %d\n", pos, runner->size);					
					printf("mmalloc: Splitting %d (%d) into %d (%d) and %d (%d)\n", pos, runner->size, pos, size, posRe, remain->size);	
					runner->size = size;
					return (void*) (runner+1);
				}
				runner = runner->next;
			}
			printf("mmalloc: Calling sbrk..\n");
			node *current;
			current = (node*) sbrk(1024);
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

	printf("TAIL: %d\n", (int) tail-heapStart);
	return (void*) n;
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
	printf("MAIN: Starting Main()\n");
	heapStart = (int) sbrk(0);
	head = NULL;
	tail = NULL;
	printFreeList();
	printf("MAIN: Testing imperfect match..\n");
	int* x =(int*)  mmalloc(12);
	for(int i = 0; i < 3; i++) {
		x[i] = 42;
		printf("%d\n", x[i]);
	}
	
	printFreeList();	
	printf("MAIN: Testing perfect match..\n");
	void* p = mmalloc(1012);
	printFreeList();	
	printf("MAIN: free perfect match..\n");
	mfree(p);
	printFreeList();
	printf("MAIN: Testing imperfect match..\n");
	mmalloc(50);
	printFreeList();
	mfree(x);
	printFreeList();
}
