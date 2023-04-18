#include <mpi.h>
#include <iostream>

int main (int argc, char* argv[]) {

  MPI_Init (&argc, &argv);

  int size;
  int rank;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (argc < 2) {
    std::cerr<<"usage: mpirun "<<argv[0]<<" <value>"<<std::endl;
    return -1;
  }
  
  float val;
  
  if (rank == 0) {
    MPI_Status s;
    sscanf(argv[1], "%f", &val); // Read value
    MPI_Send(&val, 1, MPI_FLOAT, 1, 123, MPI_COMM_WORLD); // Send to Processor 1
    MPI_Recv(&val, 1, MPI_FLOAT, 1, 321, MPI_COMM_WORLD, &s); // Recieve from Process 1
    std::cout << val << std::endl; // Print value
  }
  else if (rank == 1) {
    MPI_Status s;
    MPI_Recv(&val, 1, MPI_FLOAT, 0, 123, MPI_COMM_WORLD, &s); // Recieve from Process 0
    val += 2;
    MPI_Send(&val, 1, MPI_FLOAT, 0, 321, MPI_COMM_WORLD); // Send to Processor 0
  }

  MPI_Finalize();
  return 0;
}
