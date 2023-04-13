#include <mpi.h>
#include <unistd.h>
#include <iostream>

#define HOST_NAME_LENGTH 128

int main(int argc, char*argv[]) {
    MPI_Init (&argc, &argv);

    int size;
    int rank;
    char hostname[HOST_NAME_LENGTH];

	gethostname(hostname, HOST_NAME_LENGTH);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::cout<<"I am process "<<rank+1<<" out of "<<size<<". I am running on "<<hostname<<".\n";

    MPI_Finalize(); 
}
