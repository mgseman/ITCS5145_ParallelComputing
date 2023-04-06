CFLAGS=-O3 -std=c11 -fPIC -g  -Xassembler -mrelax-relocations=no
CXXFLAGS=-O3 -std=c++17 -fPIC -g -Xassembler -mrelax-relocations=no
LDFLAGS=-fPIE
LD=g++

all: activity-prefixsum.tgz

libgen.a: gen_lib.o
	ar rcs libgen.a gen_lib.o

%.pdf: %.tex
	pdflatex $<

ARXIV=Makefile approx.cpp \
      libgen.a \
      sequential/prefixsum_seq.cpp sequential/bench_sequential.sh sequential/queue.sh sequential/Makefile \
      prefixsum/plot.sh prefixsum/test.sh prefixsum/prefixsum.cpp prefixsum/omploop.hpp prefixsum/bench_prefixsum.sh prefixsum/queue.sh prefixsum/Makefile \
      .gitignore params.sh \
      activity_prefixsum_loops.pdf

activity-prefixsum.tgz: $(ARXIV)
	tar zcvf $@ \
                 $(ARXIV)

