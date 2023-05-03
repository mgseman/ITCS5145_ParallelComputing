CXX=mpicxx
CC=mpicxx
LD=mpicxx
CXXFLAGS=-O2 -std=c++11
LDLIBS=libfunctions.a 

mpi_advanced_manager_worker: mpi_advanced_manager_worker.o
  
run: mpi_advanced_manager_worker
	mpirun -oversubscribe -np 2 ./mpi_advanced_manager_worker 1 0 10 1000 1

bench: mpi_advanced_manager_worker
	./queue.sh

plot:
	./plot.sh

test: mpi_advanced_manager_worker ../approx
	./test.sh

clean:
	-rm *.o
	-rm mpi_advanced_manager_worker

distclean:
	rm *.out
