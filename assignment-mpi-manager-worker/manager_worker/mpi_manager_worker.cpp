#include <mpi.h>
#include <chrono>
#include <iostream>

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

#define KEEP_WORKING 0
#define NO_MORE_JOBS 1
#define READY_TO_WORK 2
#define MANAGER_RANK 0

int main (int argc, char* argv[]) {

  if (argc < 6) {
    std::cerr<<"usage: mpirun -np <processes> "<<argv[0]<<" <functionid> <a> <b> <n> <intensity>"<<std::endl;
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
  float answer = 0;
  float sum = 0;
  float locsum = 0;
  int* strend = new int[2];
  int granularity = n / 100;

  if (rank == MANAGER_RANK) { // Manager
    MPI_Status s;
    int start = 0;
    int tag = 1;
    int placeholder = 0;
    while (start < n) {
      // Get available workers
      MPI_Recv(&placeholder, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &s);
      if (s.MPI_TAG == READY_TO_WORK) {
        strend[0] = start;
        if (start + granularity < n) {
          strend[1] = start + granularity;
          start = strend[1];
        }
        else {
          strend[1] = n;
          start = strend[1];
        }
        MPI_Send(&(strend[0]), 2, MPI_INT, s.MPI_SOURCE, KEEP_WORKING, MPI_COMM_WORLD);
      }
    }
    
    // Terminate workers
    for (int i = 1; i < size; i++) {
      MPI_Recv(&placeholder, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &s);
      if (s.MPI_TAG == READY_TO_WORK) {
        MPI_Send(&(strend[0]), 2, MPI_INT, s.MPI_SOURCE, NO_MORE_JOBS, MPI_COMM_WORLD);
      }
    }

  }
  else { // Workers
    MPI_Status s;
    int placeholder = 0;
    bool done = false;

    // Tell manager worker is ready
    MPI_Send(&placeholder, 1, MPI_INT, MANAGER_RANK, READY_TO_WORK, MPI_COMM_WORLD);

    while (!done) {
      MPI_Recv(&(strend[0]), 2, MPI_INT, MANAGER_RANK, MPI_ANY_TAG, MPI_COMM_WORLD, &s);
      if (s.MPI_TAG == KEEP_WORKING) {
        for (int i = strend[0]; i < strend[1]; i++) {
	        locsum += f((a + (i + 0.5f) * step), intensity);
        }
        // Tell manager worker is ready
        MPI_Send(&placeholder, 1, MPI_INT, MANAGER_RANK, READY_TO_WORK, MPI_COMM_WORLD);
      }
      else if (s.MPI_TAG == NO_MORE_JOBS) {
        done = true;
      }
    }
    
    
  }

  // Reduce local sums onto sum
  MPI_Reduce(&locsum, &sum, 1, MPI_FLOAT, MPI_SUM, MANAGER_RANK, MPI_COMM_WORLD);

  // Calculate answer on manager rank
  answer = step * sum;

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

  return 0;
}
