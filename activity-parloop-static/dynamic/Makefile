SRCS =  dynamic_loop.cpp dynamic_main.cpp
OBJS = $(SRCS:.cpp=.o)

CFLAGS=-O3 -std=c11 -g -fPIC -pthread
CXXFLAGS=-O3 -std=c++17 -g -fPIC -pthread -fopenmp
LDFLAGS=-pthread -fopenmp
LDLIBS=../libfunctions.a
LD=g++

.PHONY: plot dynamic_sched

all: dynamic_sched

approx:
	$(LD) $(CXXFLAGS) $(LDFLAGS) ../approx.cpp $(LDLIBS) -o ../approx

dynamic_sched: $(OBJS)
	$(LD)  $(OBJS) -o dynamic_sched $(LDFLAGS) $(LDLIBS)

bench: dynamic_sched
	./queue.sh 

test: dynamic_sched approx
	./../test.sh dynamic cases.txt

plot: 
	./plot.sh

clean:
	-rm *.o
	-rm dynamic_sched

distclean:
	rm *.out
