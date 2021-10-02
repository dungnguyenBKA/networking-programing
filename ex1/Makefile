
all: common.o exercise1 exercise2

exercise1: 
	gcc -o exercise1 exercise1.c common.o

exercise2: 
	gcc -o exercise2 exercise2.c common.o

common.o: common.h common.c
	gcc -c common.c

clean:
	rm -f exercise1 exercise2 *.out *.o