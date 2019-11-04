test: testme.c
	gcc -o test testme.c -coverage -fpic -lm -std=c99

runtest: test
	./test

clean:
	rm test
