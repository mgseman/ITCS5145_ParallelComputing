#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <chrono>
#include "omploop.hpp"

#ifdef __cplusplus
extern "C" {
#endif

  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (const int* arr, size_t n);

#ifdef __cplusplus
}
#endif

void swap(int* arr, int i, int j) {
  int temp = arr[i];
  arr[i] = arr[j];
  arr[j] = temp;
} 

int main (int argc, char* argv[]) {
  if (argc < 3) { std::cerr<<"usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int nthds = atoi(argv[2]);
  int granularity = std::max((n / 100), 1);

  // get arr data
  int * arr = new int [n];
  generateMergeSortData (arr, n);

  //insert sorting code here.

  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  OmpLoop ol;
  ol.setNbThread(nthds);
  ol.setGranularity(granularity);

  for (int sl=0; sl < 2*n - 2; ++sl) {
    int z = (sl < n) ? 0 : 2 * (sl - n + 1);
    // Parallel Loop
    /*
    for (int j = sl % 2; j <= sl - z; j += 2) {
      if (arr[j-1] > arr[j]) {
          swap(arr, j-1, j);
      }
    }
    */
    ol.parfor(sl % 2, sl - z + 1, 2,
              [&] (int j) {
                if (arr[j-1] > arr[j]) {
                  swap(arr, j-1, j);
                }
              });
      
  }

  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elpased_seconds = end-start;
  
  checkMergeSortResult (arr, n);
  std::cerr<<elpased_seconds.count()<<std::endl;
  
  delete[] arr;

  return 0;
}
