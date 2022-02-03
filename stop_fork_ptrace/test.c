#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
	int *x = NULL;
	
	
	x = calloc(1, sizeof(int));
	printf("Initial x = %d\n", *x);
	
	FILE *fp = fopen("/dev/urandom", "r");
	fread(x, sizeof(int), 1, fp);
	fclose(fp);
	fprintf(stderr, "Now x = %d\n", *x);
	
    fork(); 	
	
	*x = -1;
    printf("After fork() x = %d\n", *x);
	free(x);
	
    return 0; 
} 
