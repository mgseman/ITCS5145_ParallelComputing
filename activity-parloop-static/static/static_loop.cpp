#ifndef __STATIC_LOOP_H
#define __STATIC_LOOP_H

#include <iostream>
#include <functional>
#include <thread>
#include <mutex>
#include <cmath>

class StaticLoop {
private:
  // @brief you will want to have class member to store the number of threads
  // add a public setter function for this member.

  size_t nbthreads;
  
  template<typename TLS>
  static void thdfunc(size_t beg, size_t end, size_t increment,
          std::function<void(TLS&)> before,
	        std::function<void(int, TLS&)> f,
	       std::function<void(TLS&)> after, 
         std::mutex &mut)
  {
    TLS loc_tls = 0;
    
    before(loc_tls);

    for(int i = beg; i < end; i+=increment)
    {
      f(i, loc_tls);
    }
    
    std::lock_guard<std::mutex> lg(mut);
    after(loc_tls);
  }

public:
  // @breif write setters here.

  void setNumThreads(int nthds) {
    nbthreads = nthds;
  }

  /// @brief execute the function f multiple times with different
  /// parameters possibly in parallel
  ///
  /// f will be executed multiple times with parameters starting at
  /// beg, no greater than end, in inc increment. These execution may
  /// be in parallel
  void parfor (size_t beg, size_t end, size_t inc,
	       std::function<void(int)> f) {
    for (size_t i=beg; i<end; i+= inc) {
      f(i);
    }
  }

  /// @brief execute the function f multiple times with different
  /// parameters possibly in parallel
  ///
  /// f will be executed multiple times with parameters starting at
  /// beg, no greater than end, in inc increment. These execution may
  /// be in parallel.
  ///
  /// Each thread that participate in the calculation will have its
  /// own TLS object.
  ///
  /// Each thread will execute function before prior to any
  /// calculation on a TLS object.
  ///
  /// Each thread will be passed the TLS object it executed before on.
  ///
  /// Once the iterations are complete, each thread will execute after
  /// on the TLS object. No two thread can execute after at the same time.
  template<typename TLS>
  void parfor (size_t beg, size_t end, size_t increment,
	       std::function<void(TLS&)> before,
	       std::function<void(int, TLS&)> f,
	       std::function<void(TLS&)> after
	       ) 
  {
    size_t batch_size = (end - beg) / nbthreads;
    std::vector<std::thread> m_threads;
    std::mutex mut;
    
    for(int i = 0; i < nbthreads; i++) {
      size_t start = i * batch_size;
      size_t loc_beg = beg + start;
      size_t loc_end;

      if (i == nbthreads - 1) {
        loc_end = end;
      }
      else {
        loc_end = loc_beg + batch_size;
      }
      
      std::thread m_thread(thdfunc<TLS>, loc_beg, loc_end, increment, before, f, after, std::ref(mut));
      m_threads.push_back(std::move(m_thread));

      //m_threads.emplace_back([&](){thdfunc<TLS>(loc_beg, loc_end, increment, before, f, after, std::ref(mut));});
    }

    for (auto & t : m_threads) {
      t.join();
    }
  }
  
};

#endif
