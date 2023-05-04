#include <iostream>
#include <chrono>

__global__ void VecAdd(float *d_in1, float *d_in2, float *d_out)
{
  int i = threadIdx.x;
  d_out[i] = d_in1[i] + d_in2[i];
}

int main (int argc, char* argv[]) {
  //TODO: add usage
  
  int n = atoi(argv[1]); //TODO: atoi is an unsafe function

  float* in1 = new float[n];
  float* in2 = new float[n];
  float* out = new float[n];
  
  
  for (int i=0; i<n; ++i) {
    in1[i] = i;
    in2[i] = 100+i;
  }


  // Start Timer
  std::chrono::time_point<std::chrono::system_clock> begin, end;
  begin = std::chrono::system_clock::now();

  //gpu stuff goes here.

  // Declare and allocate GPU memory
  float *d_in1;
  float *d_in2;
  float *d_out;
  cudaMalloc((void **) &d_in1, n);
  cudaMalloc((void **) &d_in2, n);
  cudaMalloc((void **) &d_out, n);

  // Copy input arrays onto GPU memory
  cudaMemcpy(d_in1, in1, n, cudaMemcpyHostToDevice);
  cudaMemcpy(d_in2, in2, n, cudaMemcpyHostToDevice);

  // Perform vector addition on GPU
  VecAdd<<<1, n>>>(d_in1, d_in2, d_out);

  // Copy output of vector addition onto CPU memory
  cudaMemcpy(d_out, out, n, cudaMemcpyHostToDevice);
  
  // Free GPU memory
  cudaFree(d_in1);
  cudaFree(d_in2);
  cudaFree(d_out);

  end = std::chrono::system_clock::now();
  std::chrono::duration<double> totaltime = (end-begin);

  std::cout<<n<<" "<<totaltime.count()<<std::endl;

  delete[] in1;
  delete[] in2;
  delete[] out;

  return 0;
}
