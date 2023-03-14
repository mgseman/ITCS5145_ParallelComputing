SRCS =  static_loop.cpp static_main.cpp
OBJS = $(SRCS:.cpp=.o)

CFLAGS=-O3 -std=c11 -g -fPIC -pthread 
CXXFLAGS=-O3 -std=c++17 -g -fPIC -pthread -fopenmp
LDFLAGS=-pthread -fopenmp
LDLIBS=../libfunctions.a
LD=g++

.PHONY: plot static_sched

all: static_sched

static_sched: $(OBJS)
	$(LD)  $(OBJS) -o static_sched $(LDFLAGS) $(LDLIBS)

approx:
	$(LD) $(CXXFLAGS) $(LDFLAGS) ../approx.cpp $(LDLIBS) -o ../approx

bench: static_sched
	./queue.sh 

test: static_sched approx
	./../test.sh static cases.txt 

plot: 
	./plot.sh  

clean:
	-rm *.o
	-rm static_sched

distclean:
	rm *.out
