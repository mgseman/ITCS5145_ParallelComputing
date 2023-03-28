#ifndef __DYNAMIC_LOOP_H
#define __DYNAMIC_LOOP_H

#include <functional>
#include <thread>
#include <mutex>

class DynamicLoop {
private:
  // @brief you will want to have class members to store the number of threads
  // and the desired granularity. Add a public setter functions for these
  // class members.

  int nthreads;
  int granularity;

public:
  // @breif write setters here.

  void setNumThreads(int nthrds)
  {
    nthreads = nthrds;
  }

  void setGranularity(int gran)
  {
    granularity = gran;
  }

  template<typename TLS>
  static void thdfunc(size_t &beg, size_t end, size_t increment,
          std::function<void(TLS&)> before,
	        std::function<void(int, TLS&)> f,
	       std::function<void(TLS&)> after, 
         std::mutex &mut,
         size_t granularity)
  {
    TLS loc_tls = 0;
    
    before(loc_tls);

    size_t loc_beg; 
    size_t loc_end;

    while (beg < end)
    {
      mut.lock();
      loc_beg = beg;
      if (beg + granularity < end){
        loc_end = beg + granularity;
      }
      else {
        loc_end = end;
      }
      beg = beg + granularity;
      mut.unlock();
      for(size_t i = loc_beg; i < loc_end; i+=increment)
      {
        f(i, loc_tls);
      }
    }
    
    std::lock_guard<std::mutex> lg(mut);
    after(loc_tls);
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
	       ) {
    size_t batch_size = end / nthreads;
    std::vector<std::thread> m_threads;
    std::mutex mut;
    
    for(size_t i = 0; i < nthreads; i++) {
      size_t thd_start = beg;

      std::thread m_thread(thdfunc<TLS>, std::ref(thd_start), end, increment, before, f, after, std::ref(mut), granularity);
      m_threads.push_back(std::move(m_thread));

      //m_threads.emplace_back([&](){thdfunc<TLS>(loc_beg, loc_end, increment, before, f, after, std::ref(mut));});
    }

    for (auto & t : m_threads) {
      t.join();
    }
  }
  
};

#endif
