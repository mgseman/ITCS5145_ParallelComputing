#include <iostream>
#include <chrono>

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


  
  std::chrono::time_point<std::chrono::system_clock> begin, end;
  begin = std::chrono::system_clock::now();
  
  for (int i=0; i<n; ++i) {
    out[i] = in1[i]+in2[i];
  }

  
  end = std::chrono::system_clock::now();
  std::chrono::duration<double> totaltime = (end-begin);

  std::cout<<n<<" "<<totaltime.count()<<std::endl;

  delete[] in1;
  delete[] in2;
  delete[] out;

  return 0;
}
