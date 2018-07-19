CFLAGS = -Wall -fpic -coverage -lm -std=c99

default:
	gcc -o runTest testme.c $(CFLAGS)

run: testme.c
	make && ./runTest

clean: 
	rm -rf *.gcda *.gcno runTest