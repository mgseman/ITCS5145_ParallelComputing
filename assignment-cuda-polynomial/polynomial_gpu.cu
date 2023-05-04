#include <iostream>
#include <chrono>

__global__ void polynomial_expansion (float* poly, int degree,
			   int n, float* array) {
  //TODO: Write code to use the GPU here!
  //code should write the output back to array

  int idx = blockIdx.x * blockDim.x + threadIdx.x;
  if (idx < n) 
  {
    float out = 0.;
    float xtothepowerof = 1.;
    for (int i=0; i<=degree; ++i) {
      out += xtothepowerof*poly[i];
      xtothepowerof *= array[idx];
    }
    array[idx] = out;
  }
}


int main (int argc, char* argv[]) {
  if (argc < 3) {
     std::cerr<<"usage: "<<argv[0]<<" n degree"<<std::endl;
     return -1;
  }

  int n = atoi(argv[1]); //TODO: atoi is an unsafe function
  int degree = atoi(argv[2]);
  int nbiter = 1;

  float* array = new float[n];
  float* poly = new float[degree+1];
  for (int i=0; i<n; ++i)
    array[i] = 1.;

  for (int i=0; i<degree+1; ++i)
    poly[i] = 1.;

  
  std::chrono::time_point<std::chrono::system_clock> begin, end;
  begin = std::chrono::system_clock::now();

  float *d_poly;
  float *d_arr;
  cudaMalloc((void **) &d_poly, (degree+1)*sizeof(float));
  cudaMalloc((void **) &d_arr, n*sizeof(float));

  // Copy input arrays onto GPU memory
  cudaMemcpy(d_poly, poly, (degree+1)*sizeof(float), cudaMemcpyHostToDevice);
  cudaMemcpy(d_arr, array, n*sizeof(float), cudaMemcpyHostToDevice);

  // Perform Polynomial expansion on GPU
  for (int iter = 0; iter<nbiter; ++iter) {
    polynomial_expansion<<<(n+255)/256, 256>>>(d_poly, degree, n, d_arr);
  }
  
  // Send array back to CPU memory
  cudaMemcpy(array, d_arr, n*sizeof(float), cudaMemcpyDeviceToHost);

  // Free CUDA memory
  cudaFree(d_arr);
  cudaFree(d_poly);

  end = std::chrono::system_clock::now();
  std::chrono::duration<double> totaltime = (end-begin)/nbiter;

  {
    bool correct = true;
    int ind;
    for (int i=0; i< n; ++i) {
      if (fabs(array[i]-(degree+1))>0.01) {
        correct = false;
	      ind = i;
      }
    }
    if (!correct)
      std::cerr<<"Result is incorrect. In particular array["<<ind<<"] should be "<<degree+1<<" not "<< array[ind]<<std::endl;
  }
  

  std::cerr<<array[0]<<std::endl;
  std::cout<<n<<" "<<degree<<" "<<totaltime.count()<<std::endl;

  delete[] array;
  delete[] poly;

  return 0;
}
