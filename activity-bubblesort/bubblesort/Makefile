CFLAGS=-O3 -std=c11 -fPIC -g 
CXXFLAGS=-O3 -std=c++17 -fPIC -g -fopenmp -pthread
ARCHIVES=../libgen.a 
LD=g++
LDFLAGS=-pthread -fopenmp

all: bubblesort 


bubblesort: bubblesort.o
	$(LD) $(LDFLAGS) bubblesort.o $(ARCHIVES) -o bubblesort

test: bubblesort
	./test.sh

bench: bubblesort
	./queue.sh

plot:  
	./plot.sh

clean:
	-rm *.o
	-rm bubblesort

distclean:
	rm *.out


.PHONY:  bubblesort
