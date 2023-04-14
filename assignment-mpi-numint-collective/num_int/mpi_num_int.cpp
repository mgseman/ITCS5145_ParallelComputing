#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <mpi.h>

#ifdef __cplusplus
extern "C" {
#endif

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

#ifdef __cplusplus
}
#endif

  
int main (int argc, char* argv[]) {
  
  if (argc < 6) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity>"<<std::endl;
    return -1;
  }
  
  //Start Clock
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  MPI_Init (&argc, &argv);

  int size;
  int rank;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int functionid;
  double a, b;
  long long int n; 
  int intensity;
  int nbthreads;

  sscanf(argv[1], "%d", &functionid); // argv[1] is int functionid
  sscanf(argv[2], "%lf", &a);         // argv[2] is double a
  sscanf(argv[3], "%lf", &b);         // argv[3] is double b
  sscanf(argv[4], "%lld", &n);        // argv[4] is int64 n (# of points)
  sscanf(argv[5], "%d", &intensity);  // argv[5] is int intensity

  // Rectangle step size (b - a) / n
  float step = (b - a) / n;

  //Declare function pointer
  float (*f)(float,int);
  
  //Choose function
  switch (functionid)
  {
  case 1:
    f = &f1;
    break;
  case 2:
    f = &f2;
    break;
  case 3:
    f = &f3;
    break;
  case 4:
    f = &f4;
    break;
  default:
    fputs("Invalid functionid.\n", stdout);
    abort(); //Abort program
    break;
  }

  // Summation  
  float sum = 0;
  float locsum = 0;

  int thdstart = rank * (n / size);
  int thdend = n;
  if (rank == size - 1) {
	thdend = n;
  }
  else {
	thdend = (rank + 1) * (n / size);
  }

  for (int i = thdstart; i < thdend; i++) {
	locsum += f((a + (i + 0.5f) * step), intensity);
  }

  MPI_Reduce(&locsum, &sum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

  float answer = step * sum;

  //End Clock
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	
  //Record time taken
  float time = std::chrono::duration<float>(end - begin).count();

  if (rank == 0)
  {
  	// Output answer on stdout
  	std::cout << answer << std::endl;

  	// Output time on stderr
  	std::cerr << time << std::endl;
  }

  MPI_Finalize();
}
