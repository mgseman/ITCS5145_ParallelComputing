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
  
  OmpLoop ol;

  int** C = new int*[m+1];
  for (int i=0; i<=m; ++i) {
    C[i] = new int[n+1];
    C[i][0] = 0;
  }
  for (int j=0; j<=n; ++j) {
    C[0][j] = 0;
  }

  ol.setNbThread(num_threads);
  ol.setGranularity((n+m)/200);

  int result = 0;
  
  for (int i=1; i<=m+n; ++i) {
    ol.parfor<int**>(1, i+1, 1, 
                  [&C](int**& tls){
                    tls = C;
                  }, [&X, &m, &Y, &n, &i](int b, int**& tls){
                    int a = i - b;
                    if (a <= m && a > 0 && b <= n && b > 0)
                    {
                      if (X[a-1] == Y[b-1]) {
                        tls[a][b] = tls[a-1][b-1] + 1; 
                      } else {
                        tls[a][b] = std::max(tls[a-1][b], tls[a][b-1]);
                      }
                    }
                  
                  }, [&result, &m, &n](int**& tls){
                    result = tls[m][n];
                  });
  }

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
