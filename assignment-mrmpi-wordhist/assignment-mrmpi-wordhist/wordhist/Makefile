CXX=mpicxx
CC=mpicxx
LD=mpicxx

HOSTN=$(shell hostname)

CXXMRMPIFLAGS=$(shell if [ "$(HOSTN)" = "gal-i1.uncc.edu" ] ; then echo -I /projects/class/itcs5145_001/mapreduce-7Apr14/src; else echo -I /usr/include/mrmpi ; fi )

LDMRMPILIBS=$(shell if [ "$(HOSTN)" = "gal-i1.uncc.edu" ] ; then echo /projects/class/itcs5145_001/mapreduce-7Apr14/src/libmrmpi_mpicc.so; else echo -lMapReduceMPI ; fi )

CXXFLAGS=-O2 $(CXXMRMPIFLAGS) -std=c++11
LDFLAGS=$(LDMRMPIFLAGS)
LDLIBS=$(LDMRMPILIBS)


wordhist: wordhist.o

bench: wordhist
	./queue.sh

test: wordhist
	./test.sh

plot:
	./plot.sh

clean:
	-rm *.o
	-rm *.sh.*
	-rm wordhist

