#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>

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

// Compute numerical integration from point a to b
// using rectangular method for function identified
// by functionid
int main (int argc, char* argv[]) {

  //Start Clock
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  // argv[1] is int functionid
  int functionid;
  sscanf(argv[1], "%d", &functionid);

  // argv[2] is double a, argv[3] is double b
  double a, b;
  sscanf(argv[2], "%lf", &a); // %lf reads double
  sscanf(argv[3], "%lf", &b); 

  // argv[4] is int64 n (# of points)
  long long int n; 
  sscanf(argv[5], "%lld", &n); // %lld reads 64 bit int
  
  // argv[5] is int intensity
  int intensity;
  sscanf(argv[5], "%d", &intensity);

  // Rectangle step size (b - a) / n
  float step = (b - a) / n;

  // Summation
  float sum = 0;
  
  switch (functionid)
  {
  case 1:
    for (int i = 0; i < n-1; i++) {

      // Function input x
      float x = a + (i + 0.5f) * step;

      sum += f1(x, intensity);
    }
    break;
  case 2:
    for (int i = 0; i < n-1; i++) {

      // Function input x
      float x = a + (i + 0.5f) * step;

      sum += f2(x, intensity);
    }
    break;
  case 3:
    for (int i = 0; i < n-1; i++) {

      // Function input x
      float x = a + (i + 0.5f) * step;

      sum += f3(x, intensity);
    }
    break;
  case 4:
    for (int i = 0; i < n-1; i++) {

      // Function input x
      float x = a + (i + 0.5f) * step;

      sum += f4(x, intensity);
    }
    break;
  default:
    fputs("Invalid functionid.\n", stderr);
    abort(); //Abort program
    break;
  }

  float answer = step * sum;

  //End Clock
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

  //Record time taken
  int time = std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();

  // Output answer on stdout
  std::cout << answer;

  // Output time on stderr
  std::cerr << time;

  if (argc < 6) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity>"<<std::endl;
    return -1;
  }
  
  return 0;
}
