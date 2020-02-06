#include "PCalc_T.h"
#include <unistd.h>
#include <math.h>
#include <thread>
#include <mutex>
#include <iostream>

PCalc_T::PCalc_T(unsigned int array_size, unsigned int num_threads): PCalc(array_size) {
    this->num_threads = num_threads;
}


void PCalc_T::markNonPrimes()  {
    // Start Looping through numbers
    for(int i = 2; i <= sqrt(this->array_size()); i++) {
        // If the number is a prime number up to this point
        if(this->at(i)) {
            // This loop will check if our main thread is behind our worker threads
            bool min = false;
            do {
                min = false;
                for(auto min_thd: curr_thds) {
                    if(min_thd <= i) {
                        min = false;
                        usleep(0.000001);
                    }
                }
            } while(min);

            // Push the number onto a list to get used by the threads
            current_prime = i;

            // Spawn the number of threads
            if(thread_pool.size() < num_threads) {
                curr_thds.push_back(i);
                std::thread thd = std::thread([=]{threadNonPrimes(curr_thds.size() - 1);});
                thread_pool.push_back(std::move(thd));
            }
        }
    }
    // Tell the threads to stop working
    marking = false;

    for(auto& thd : thread_pool ) {
        if(thd.joinable()) {
            thd.join();
        }
    }
}

void PCalc_T::threadNonPrimes(int id) {
    int i = 0;
    while(marking) {
        // Check if the list is empty
        while(current_prime == 1) {
            if(!marking)
                break;
        }
        // std::cout << "Thread " << id << " ready to read prime\n";
        {
            std::lock_guard<std::mutex> lock(thd_lock);
            i = current_prime;
            current_prime = 1;
        }

        for(int j = pow(i,2); j < this->array_size(); j += i) {
            this->at(j) = false; 
            curr_thds.at(id) = j;
        }

        // std::cout << "Thread " << id << " finished work on " << i << "\n";
        curr_thds.at(id) = array_size();
    }
}


            