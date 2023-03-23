#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <cmath>
#include "static_loop.cpp"


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

  if (argc < 7) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads>"<<std::endl;
    return -1;
  }
  
  //Start Clock
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

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
  sscanf(argv[6], "%d", &nbthreads);  // argv[6] is the number of threads

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

  StaticLoop sl;

  sl.setNumThreads(nbthreads);

  // Summation
  float sum = 0;

  sl.parfor<float>(0, 
                 n, 
                 1,
                 [&](float& tls) {tls = 0;},
                 [&](int i, float& tls) {
                    tls += f((a + (i + 0.5f) * step), intensity);
                    },
                 [&](float& tls){
                  sum += tls;
                  }
                );

  float answer = step * sum;

  //End Clock
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

  //Record time taken
  float time = std::chrono::duration<float>(end - begin).count();

  // Output answer on stdout
  std::cout << answer << std::endl;

  // Output time on stderr
  std::cerr << time << std::endl;

  return 0;
}
