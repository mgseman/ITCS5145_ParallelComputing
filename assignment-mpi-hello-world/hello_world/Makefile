CXX=mpicxx
CC=mpicxx
LD=mpicxx

all: hello

run: hello
	mpirun -np 1 ./hello

bench: hello
	./queue.sh


clean:
	-rm *.o
	-rm hello

distclean:
	rm *.out
