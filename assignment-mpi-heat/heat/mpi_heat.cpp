#include <mpi.h>
#include <chrono>
#include <math.h>
#include <vector>
#include <iostream>


#ifdef __cplusplus
extern "C" {
#endif
  double generate2DHeat(long n, long global_i, long global_j);

  int check2DHeat(long n, long global_i, long global_j, double v, long k); //this function return 1 on correct. 0 on incorrect. Note that it may return 1 on incorrect. But a return of 0 means it is definitely incorrect
#ifdef __cplusplus
}
#endif

void showHeat(double **H, int m, int n) {
  for (int i=0; i<m; ++i) {
    for (int j=0; j<n; ++j) {
      std::cout<<H[i][j]<<" ";
    }
    std::cout<<std::endl;
  }
}

/**
 *
 */
double** prev_heat(double ***sub_heat, int k) {
  return sub_heat[(k-1)%2];
}


/**
 *
 */
double** curr_heat(double ***sub_heat, int k) {
  return sub_heat[k%2];
}

/**
 * block[9] 
 *
 * 0 1 2
 * 3 4 5   ~~~ (i, j) is position 4
 * 6 7 8
 *
 */
void heat_eq(double **prev, double **curr, int i, int j, int N, double *upper_bnd, double *lower_bnd, double *right_bnd, double *left_bnd) {

  int stencil = 5;
  double *block = new double[stencil];
  // init all to center of block, prev[i][j] 
  for (int t=0; t<stencil; ++t) {
    block[t] = prev[i][j]; 
  }
  
  // column LEFT of center
  if ((j-1) >= 0) {
    block[1] = prev[i][j-1];
  }
  else {
    block[1] = left_bnd[i];
  }
  
  // above center
  if ((i-1) >= 0) {
    block[0] = prev[i-1][j];
  }
  else {
    block[0] = upper_bnd[j];
  }
  // below center
  if ((i+1) < N) {
    block[4] = prev[i+1][j];
  }
  else {
    block[4] = lower_bnd[j];
  } 
  
  // column RIGHT of center
  if ((j+1) < N) {
    block[3] = prev[i][j+1];
  }
  else {
    block[3] = right_bnd[i];
  }
  
#if DEBUG //
  std::cout<<"i: "<<i;
  std::cout<<", j: "<<j<<" = [ ";
  for (int t=0; t<9; ++t) {
    if (t % 3 == 0) std::cout<<std::endl;
    std::cout<<block[t]<<" ";
  }
  std::cout<<std::endl<<"]"<<std::endl<<std::endl;
#endif

  // reset center of block i,j
  curr[i][j] = 0.0f; 
  // sum up block
  for (int t=0; t<stencil; ++t) {
    curr[i][j] += block[t];
  }
  // average
  curr[i][j] /= (float) stencil;

  delete[] block;
}

int main(int argc, char* argv[]) {

  if (argc < 3) {
    std::cerr<<"usage: mpirun "<<argv[0]<<" <N> <K>"<<std::endl;
    return -1;
  }

  MPI_Init (&argc, &argv);

  int size;
  int rank;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // declare and init command line params
  long N, K;
  N = atol(argv[1]);
  K = atol(argv[2]);

  // use double for heat 2d 

  double ***sub_heat = new double**[2];

  int sub_N = N/sqrt(size);

  for (int k=0; k<2; ++k) {
    sub_heat[k] = new double*[sub_N];
    for (int i=0; i<sub_N; ++i) { // dont init buffers, i==0, i==m-1
      sub_heat[k][i] = new double[sub_N];
    }
#if DEBUG
    if (k == 0) showHeat(sub_heat[k], N, N);
#endif
  }

  long row = rank%lround(sqrt(size));
  long col = rank/lround(sqrt(size));
  
  // generate 2D array
  //generate2DHeat(sub_heat[0], N, rank, P);
  {
    double** h = sub_heat[0];
    for (long i = 0; i<sub_N; ++i)
      for (long j = 0; j<sub_N; ++j) {
	      long global_i = i + sub_N * row;
	      long global_j = j + sub_N * col;
	      h[i][j] = generate2DHeat(N, global_i, global_j);
      }
	  
  }

  //showHeat(sub_heat[0], sub_N, sub_N);


  // write code here

  // convient handles for Heat^k-1, Heat^k
  double **prev, **curr;

  // begin timing
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  double* upper_bnd_send = new double[sub_N];
  double* lower_bnd_send = new double[sub_N];
  double* right_bnd_send = new double[sub_N];
  double* left_bnd_send = new double[sub_N];

  double* upper_bnd_recv = new double[sub_N];
  double* lower_bnd_recv = new double[sub_N];
  double* right_bnd_recv = new double[sub_N];
  double* left_bnd_recv = new double[sub_N];

  // repeat K times
  for (int k=1; k<=K; ++k) {

    prev = prev_heat(sub_heat, k); // Heat^k-1
    curr = curr_heat(sub_heat, k); // Heat^k

    // create boundary arrays

    for(int i = 0; i < sub_N; i++) {
      // Initialize recieve bounds to zero
      upper_bnd_recv[i] = 0;
      lower_bnd_recv[i] = 0;
      right_bnd_recv[i] = 0;
      left_bnd_recv[i] = 0;
      // Boundaries to Send
      upper_bnd_send[i] = prev[0][i];
      lower_bnd_send[i] = prev[sub_N-1][i];
      left_bnd_send[i] = prev[i][0];
      right_bnd_send[i] = prev[i][sub_N-1];
    }

    int u_rank = rank - 1;
    int b_rank = rank + 1;
    bool u_rank_exist = (u_rank >= 0) && (u_rank/lround(sqrt(size)) == col);
    bool b_rank_exist = (b_rank < size) && (b_rank/lround(sqrt(size)) == col);
    int r_rank = rank + lround(sqrt(size));
    int l_rank = rank - lround(sqrt(size));

    // Comminicate through even/odd distribution
    MPI_Status s;
    // 1. Even and odd columns exchange bounds
    if (col % 2 == 0) { // Even columns
      if (r_rank < size) { // Right neighbor exists
        MPI_Send(&(right_bnd_send[0]), (sub_N), MPI_DOUBLE, r_rank, 111, MPI_COMM_WORLD);
        MPI_Recv(&(right_bnd_recv[0]), (sub_N), MPI_DOUBLE, r_rank, 114, MPI_COMM_WORLD, &s);
      }
      if (l_rank >= 0) { // Left neighbor exists
        MPI_Send(&(left_bnd_send[0]), (sub_N), MPI_DOUBLE, l_rank, 112, MPI_COMM_WORLD);
        MPI_Recv(&(left_bnd_recv[0]), (sub_N), MPI_DOUBLE, l_rank, 113, MPI_COMM_WORLD, &s);
      }
    }
    else { // Odd columns
      if (l_rank >= 0) { // Left neighbor exists
        MPI_Recv(&(left_bnd_recv[0]), (sub_N), MPI_DOUBLE, l_rank, 111, MPI_COMM_WORLD, &s);
        MPI_Send(&(left_bnd_send[0]), (sub_N), MPI_DOUBLE, l_rank, 114, MPI_COMM_WORLD);
      }
      if (r_rank < size) { // Right neighbor exists
        MPI_Recv(&(right_bnd_recv[0]), (sub_N), MPI_DOUBLE, r_rank, 112, MPI_COMM_WORLD, &s);
        MPI_Send(&(right_bnd_send[0]), (sub_N), MPI_DOUBLE, r_rank, 113, MPI_COMM_WORLD);
      }
    }

    // 2. Even and Odd rows exchange bounds
    if (row % 2 == 0) { // Even rows
      if (u_rank_exist) { // Upper neighbor exists
        MPI_Send(&(upper_bnd_send[0]), (sub_N), MPI_DOUBLE, u_rank, 115, MPI_COMM_WORLD);
        MPI_Recv(&(upper_bnd_recv[0]), (sub_N), MPI_DOUBLE, u_rank, 118, MPI_COMM_WORLD, &s);
      }
      if (b_rank_exist) { // Lower neighbor exists
        MPI_Send(&(lower_bnd_send[0]), (sub_N), MPI_DOUBLE, b_rank, 116, MPI_COMM_WORLD);
        MPI_Recv(&(lower_bnd_recv[0]), (sub_N), MPI_DOUBLE, b_rank, 117, MPI_COMM_WORLD, &s);
      }
    }
    else { // Odd rows
      if (b_rank_exist) { // Lower neighbor exists
        MPI_Recv(&(lower_bnd_recv[0]), (sub_N), MPI_DOUBLE, b_rank, 115, MPI_COMM_WORLD, &s);
        MPI_Send(&(lower_bnd_send[0]), (sub_N), MPI_DOUBLE, b_rank, 118, MPI_COMM_WORLD);
      }
      if (u_rank_exist) { // Upper neighbor exists
        MPI_Recv(&(upper_bnd_recv[0]), (sub_N), MPI_DOUBLE, u_rank, 116, MPI_COMM_WORLD, &s);
        MPI_Send(&(upper_bnd_send[0]), (sub_N), MPI_DOUBLE, u_rank, 117, MPI_COMM_WORLD);
      }
    }

    // Calculate Heat equation
    for (int i=0; i<sub_N; ++i) { 
      for (int j=0; j<sub_N; ++j) {
        heat_eq(prev, curr, i, j, sub_N, upper_bnd_recv, lower_bnd_recv, right_bnd_recv, left_bnd_recv);
      }
    }
    
  
   {
     double** h = curr;
     bool correct = true;
     for (long i = 0; i<sub_N; ++i)
       for (long j = 0; j<sub_N; ++j) {
	      long global_i = i + sub_N * row;
	      long global_j = j + sub_N * col;
	      correct &= check2DHeat(N, global_i, global_j, h[i][j], k);
	      if (!correct) {
	        std::cerr<<"incorrect computation at iteration ("<<global_i<<","<<global_j<<") "<<k<<" new check"<<std::endl;
	      }

      }
    }

#if DEBUG
    showHeat(curr, sub_N, sub_N);
    std::cout<<"_________________________"<<std::endl;
#endif
  }

  // end timing
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;

  if (rank == 0)
  {
  	// report time
    std::cerr<<elapsed_seconds.count()<<std::endl;
  }
  
  MPI_Finalize();
  return 0;
}

