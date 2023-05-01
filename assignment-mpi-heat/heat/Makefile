CXX=mpicxx
CC=mpicxx
LD=mpicxx
CFLAGS=-O2 -std=c11 -fPIC -g
CXXFLAGS=-O2 -std=c++11 -fPIC -g
ARCHIVES=libgen.a
LDLIBS=libgen.a

mpi_heat: mpi_heat.o

run: mpi_heat
	echo "not implemented"

test: mpi_heat
	./test.sh

plot: mpi_heat
	./plot.sh

libgen.a: gen_lib.o
	ar rcs libgen.a gen_lib.o

bench: mpi_heat
	./queue.sh

clean:
	-rm mpi_heat.o
	-rm mpi_heat

distclean:
	rm *.out
