CFLAGS=-O3 -std=c11 -fPIC -g
CXXFLAGS=-O3 -std=c++17 -fPIC -g
ARCHIVES=../libgen.a
LD=g++


all: prefixsum_seq

prefixsum_seq: prefixsum_seq.o
	$(LD) $(LDFLAGS) prefixsum_seq.o $(ARCHIVES) -o prefixsum_seq


bench:  prefixsum_seq
	./queue.sh

clean:
	-rm *.o
	-rm  prefixsum_seq 

distclean:
	-rm *.sh.*

.PHONY: prefixsum_seq 
