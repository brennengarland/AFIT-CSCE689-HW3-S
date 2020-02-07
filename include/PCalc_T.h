#ifndef PCALC_T_H
#define PCALC_T_H

#include "PCalc.h"
#include <vector>
#include <list>
#include <thread>
#include <mutex>

// Your implementation of PCalc_T class should go here. 
// Make sure your constructor takes in two parameters:

// PCalc_T::PCalc_T(unsigned int array_size, unsigned int num_threads);

// Call the parent constructor when initializing your constructor and pass in array_size. Then
// use num_threads to cap off how many threads you use to calculate all prime numbers

class PCalc_T : public PCalc
{
    public:
    void markNonPrimes() override;
    PCalc_T(unsigned int array_size, unsigned int num_threads);

    private:
    std::vector<std::thread> thread_pool;
    unsigned int num_threads = 0;
    std::mutex thd_lock;
    std::vector<unsigned int> curr_thds;
    std::vector<unsigned int> wrk_thds;
    unsigned int current_prime = 1;
    bool marking = true;
};


#endif
