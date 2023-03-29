CFLAGS=-O3 -std=c11 -fPIC -g
CXXFLAGS=-O3 -std=c++17 -fPIC -g -fopenmp -pthread
ARCHIVES=../libgen.a 
LD=g++
LDFLAGS=-pthread -fopenmp

all: lcs

lcs: lcs.o
	$(LD) $(LDFLAGS) lcs.o $(ARCHIVES) -o lcs

test: lcs
	./test.sh

bench: lcs
	./queue.sh

plot:  
	./plot.sh

clean:
	-rm *.o
	-rm lcs

distclean:
	rm *.out


.PHONY: lcs
