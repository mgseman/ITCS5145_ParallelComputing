CFLAGS=-O3 -std=c11 -fPIC -g -Xassembler -mrelax-relocations=no
CXXFLAGS=-O3 -std=c++17 -fPIC -g -Xassembler -mrelax-relocations=no
ARCHIVES=libgen.a 
LD=g++


all: activity-lcs.tgz


# archives
libgen.a: gen_lib.o
	ar rcs libgen.a gen_lib.o

%.pdf: %.tex
	pdflatex $<

ARXIV=Makefile \
      libgen.a \
      sequential/bench_sequential.sh sequential/lcs_seq.cpp  sequential/Makefile  sequential/queue.sh \
      lcs/bench_lcs.sh  lcs/lcs.cpp  lcs/Makefile  lcs/omploop.hpp  lcs/plot.sh  lcs/queue.sh  lcs/test.sh  \
      .gitignore params.sh \
      activity_lcs_loops.pdf

activity-lcs.tgz: $(ARXIV)
	tar zcvf $@ \
                 $(ARXIV)
