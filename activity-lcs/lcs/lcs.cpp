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

  void generateLCS(char* X, int m, char* Y, int n);
  void checkLCS(char* X, int m, char* Y, int n, int result);

#ifdef __cplusplus
}
#endif

int LCS(char* X, int m, char* Y, int n, int num_threads) {
  
  int** C = new int*[m+1];
  for (int i=0; i<=m; ++i) {
    C[i] = new int[n+1];
    C[i][0] = 0;
  }
  for (int j=0; j<=n; ++j) {
    C[0][j] = 0;
  }

  const int granularity = std::max(((n+m)/200), 1);
  OmpLoop ol;
  ol.setNbThread(num_threads);
  ol.setGranularity(granularity);

  int result = 0;
  
  for (int i=1; i<=m+n; ++i) {
    ol.parfor(1, i+1, 1, 
                    [&X, &m, &Y, &n, &i, &C](int b){
                    int a = i - b;
                    if (a <= m && a > 0 && b <= n && b > 0)
                    {
                      if (X[a-1] == Y[b-1]) {
                        C[a][b] = C[a-1][b-1] + 1; 
                      } else {
                        C[a][b] = std::max(C[a-1][b], C[a][b-1]);
                      }
                    }
                  });
  }

  result = C[m][n];

  for (int i=0; i<=m; ++i) { 
    delete[] C[i];
  }
  delete[] C;
  
  return result;
}

int main (int argc, char* argv[]) {

  if (argc < 4) { std::cerr<<"usage: "<<argv[0]<<" <m> <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int m = atoi(argv[1]);
  int n = atoi(argv[2]);
  int nthds = atoi(argv[3]);

  // get string data 
  char *X = new char[m];
  char *Y = new char[n];
  generateLCS(X, m, Y, n);

  //insert LCS code here.

  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  int lcs = LCS(X, m, Y, n, nthds);

  int result = lcs; // length of common subsequence

  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elpased_seconds = end-start;

  //End LCS code

  checkLCS(X, m, Y, n, result);
  std::cerr<<elpased_seconds.count()<<std::endl;

  delete[] X;
  delete[] Y;

  return 0;
}
