CFLAGS=-O3 -std=c17 -fPIC -Xassembler -mrelax-relocations=no
CXXFLAGS=-O3 -std=c++17 -fPIC -Xassembler -mrelax-relocations=no

LDFLAGS=-fPIE
LDLIBS=libfunctions.a
LD=g++

main: main.o
	$(LD) $(LDFLAGS) main.o $(LDLIBS) -o main

libfunctions.a: functions.o
	ar rcs libfunctions.a functions.o

.PHONY: plot test bench clean

plot:
	./plot.sh

test: main approx 
	./test.sh

bench: main 
	./queue_job.sh

clean:
	-rm main approx prelim
	-rm main.o prelim.o

%.pdf: %.tex
	pdflatex $<

ARXIVFILES=activity-numericalintegration.pdf \
	main.cpp libfunctions.a Makefile \
	approx.cpp test.sh test_cases.txt \
	queue_job.sh bench.sh run_bench.sh plot.sh .gitignore

activity-numericalintegration.tgz: $(ARXIVFILES)
	tar zcvf $@ $(ARXIVFILES)
