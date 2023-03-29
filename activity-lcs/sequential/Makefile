CFLAGS=-O3 -std=c11 -fPIC -g
CXXFLAGS=-O3 -std=c++17 -fPIC -g
ARCHIVES=../libgen.a 
LD=g++


UTILDIR=util


all: lcs_seq


# archives

libgen.a: gen_lib.o
	ar rcs libgen.a gen_lib.o


lcs_seq: lcs_seq.o
	$(LD) $(LDFLAGS) lcs_seq.o $(ARCHIVES) -o lcs_seq


# run benchmarks
bench: lcs_seq
	./queue.sh

clean:
	-rm *.o
	-rm lcs_seq

distclean:
	-rm *.sh.*


.PHONY:  lcs_seq 
