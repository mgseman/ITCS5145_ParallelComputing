#include "mpi.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sys/stat.h"
#include <chrono>

#include <fstream>
#include <map>

int main(int argc, char **argv)
{
  if (argc < 2) {
    std::cerr<<"usage: ./wordfreq <filename1> <filename2> <filename3> ..."<<std::endl;
    return -1;
  }

  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  for (int fileid=1; fileid < argc; ++fileid) {
    std::string filename = std::string(argv[fileid]);
    
    std::ifstream in (filename);
    
    //count frequencies
    std::map<std::string, int> count;
    
    while (in.good()) {
      std::string s;
      in >> s;
      if (in.good()) {
	count[s]++;
      }
    }

    //generate histogram
    std::map<int, int> histogram;
    for (auto& pa : count) {
      histogram[pa.second] = histogram[pa.second]+1;
    }

    //output
    for (auto& pa : histogram) {
      std::cout<<filename<<", "<<pa.first<<", "<<pa.second<<"\n";
    }
    
  }
  
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds = end-start;

  std::cerr<<elapsed_seconds.count()<<std::endl;

  
  return 0;
  
}
