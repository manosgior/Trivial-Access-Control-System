CFLAGS = -Wall -std=gnu99

ptrace: inject test	

test: test.c
	gcc $(CFLAGS) test.c -o test

inject: inject.c
	gcc $(CFLAGS) inject.c -o inject

clean:
	rm  test inject