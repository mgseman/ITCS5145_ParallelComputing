CXX=mpicxx
CC=mpicxx
LD=mpicxx
CXXFLAGS=-std=c++11

mpi_ping_pong: mpi_ping_pong.o

run: mpi_ping_pong
	mpirun ${MPIRUN_PARAMS} ./mpi_ping_pong 3 

bench: mpi_ping_pong
	./queue.sh

clean:
	rm *.o
	rm mpi_ping_pong

distclean:
	rm *.out
