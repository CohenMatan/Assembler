main : main.o passages.o textCheck.o utils.o bin.o errors.o tables.o files.o
	gcc -g -ansi -Wall -pedantic main.o bin.o tables.o errors.o passages.o textCheck.o utils.o files.o -o main -lm

main.o : main.c utils.h textCheck.h errors.h 
	gcc -c -ansi -Wall -pedantic main.c -o main.o

passages.o : passages.c utils.h textCheck.h errors.h
	gcc -c -ansi -Wall -pedantic passages.c -o passages.o

textCheck.o : textCheck.c errors.h utils.h
	gcc -c -ansi -Wall -pedantic textCheck.c -o textCheck.o

tables.o : tables.c utils.h textCheck.h bin.h
	gcc -c -ansi -Wall -pedantic tables.c -o tables.o

utils.o : utils.c errors.h tables.h
	gcc -c -ansi -Wall -pedantic utils.c -o utils.o

bin.o : bin.c tables.h
	gcc -c -ansi -Wall -pedantic bin.c -o bin.o 

errors.o : errors.c
	gcc -c -ansi -Wall -pedantic errors.c -o errors.o

files.o : files.c tables.h bin.h
	gcc -c -ansi -Wall -pedantic files.c -o files.o

