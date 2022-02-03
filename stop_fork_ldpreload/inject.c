#include <stdio.h>

int fork() {
	printf("My fork called\n");
	
	return 0;
}