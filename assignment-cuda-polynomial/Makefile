CXXFLAGS=-std=c++11 -O3 -fopenmp -march=native -mtune=native
LDFLAGS=-fopenmp 
CUDAFLAGS=-O3 -std=c++11 -arch=sm_37

polynomial: polynomial.cpp

polynomial_gpu: polynomial_gpu.cu
	nvcc $(CUDAFLAGS) polynomial_gpu.cu -o polynomial_gpu

bench: polynomial polynomial_gpu
	./queue_cpu.sh
	./queue_gpu.sh

test: polynomial_gpu polynomial
	./queue_test_cpu.sh
	./queue_test_gpu.sh

table:
	./table.sh

clean:
	-rm polynomial polynomial_gpu

distclean:
	-rm *.out

assignment-cuda-polynomial.pdf: assignment-cuda-polynomial.tex
	pdflatex assignment-cuda-polynomial.tex

assignment-cuda-polynomial.tgz: assignment-cuda-polynomial.pdf \
                                Makefile \
                                polynomial_gpu.cu polynomial.cpp \
                                bench_cpu.sh bench_gpu.sh queue_cpu.sh queue_gpu.sh table.sh \
                                queue_test_gpu.sh test_gpu.sh queue_test_cpu.sh test_cpu.sh
	tar zcvf assignment-cuda-polynomial.tgz \
						Makefile \
						assignment-cuda-polynomial.pdf \
						polynomial_gpu.cu polynomial.cpp \
						bench_cpu.sh bench_gpu.sh queue_cpu.sh queue_gpu.sh table.sh \
						queue_test_gpu.sh test_gpu.sh queue_test_cpu.sh test_cpu.sh
