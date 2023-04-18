CXX=mpicxx
CC=mpicxx
LD=mpicxx
CXXFLAGS=-O2 -std=c++11

mpi_matmul:


test: mpi_matmul
	./test.sh

bench: mpi_matmul
	./queue.sh

plot:
	./plot.sh

clean:
	-rm *.o
	-rm mpi_matmul

distclean:
	rm *.out

.PHONY: test plot
