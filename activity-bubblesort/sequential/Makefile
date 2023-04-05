CFLAGS=-O3 -std=c11 -fPIC -g 
CXXFLAGS=-O3 -std=c++17 -fPIC -g 
ARCHIVES=../libgen.a 
LD=g++


UTILDIR=util


all: bubblesort_seq


# archives

libgen.a: gen_lib.o
	ar rcs libgen.a gen_lib.o


bubblesort_seq: bubblesort_seq.o
	$(LD) $(LDFLAGS) bubblesort_seq.o $(ARCHIVES) -o bubblesort_seq

# run benchmarks
bench:  bubblesort_seq
	./queue.sh

clean:
	-rm *.o
	-rm bubblesort_seq

distclean:
	-rm *.sh.*


.PHONY:  bubblesort bubblesort_seq
