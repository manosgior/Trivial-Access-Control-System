CFLAGS = -Wall -std=gnu99
LIBFLAGS = -shared -fPIC

ldpreload: inject test	

test: test.c
	gcc $(CFLAGS) test.c -o test

inject: inject.c
	gcc $(CFLAGS) $(LIBFLAGS) inject.c -o my_fork.so

clean:
	rm  test my_fork.so