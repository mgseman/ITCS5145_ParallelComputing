CXXFLAGS=-std=c++11 -O3 -fopenmp -march=native -mtune=native
LDFLAGS=-fopenmp 
CUDAFLAGS=-O3 -std=c++11 -arch=sm_37

cuda_vectoradd: cuda_vectoradd.cu
	nvcc $(CUDAFLAGS) cuda_vectoradd.cu -o cuda_vectoradd

cpu_vectoradd: cpu_vectoradd.cpp


bench: cpu_vectoradd cuda_vectoradd
	./queue_cpu.sh
	./queue_gpu.sh

table:
	./table.sh

clean:
	-rm cpu_vectoradd
	-rm cuda_vectoradd

distclean:
	-rm *.out

cuda-vectoradd.pdf: cuda-vectoradd.tex
	pdflatex cuda-vectoradd.tex

assignment-cuda-vectoradd.tgz: cuda-vectoradd.pdf \
                                Makefile \
                                cuda_vectoradd.cu cpu_vectoradd.cpp \
                                bench_cpu.sh bench_gpu.sh queue_cpu.sh queue_gpu.sh table.sh
	tar zcvf assignment-cuda-vectoradd.tgz \
						Makefile \
						cuda-vectoradd.pdf \
						cuda_vectoradd.cu cpu_vectoradd.cpp \
						bench_cpu.sh bench_gpu.sh queue_cpu.sh queue_gpu.sh table.sh
