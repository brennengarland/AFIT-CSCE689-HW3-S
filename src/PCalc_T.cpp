#include "PCalc_T.h"
#include <unistd.h>
#include <math.h>
#include <thread>
#include <mutex>
#include <iostream>

PCalc_T::PCalc_T(unsigned int array_size, unsigned int num_threads): PCalc(array_size) {
    this->num_threads = num_threads;
    this->thread_pool.reserve(array_size);
    this->curr_thds.reserve(array_size);
}


void PCalc_T::markNonPrimes()  {

    auto worker_func = [this](unsigned int id) {
        unsigned int i = 0;
        while(this->marking) {
            if(this->current_prime != 1) {
                {
                    std::lock_guard<std::mutex> guard(this->thd_lock);
                    i = this->current_prime;
                    this->current_prime = 1;
                }
                if(i != 1) {
                    // std::cout << "\nCalculating " << i << "\n";
                    for(unsigned int j = pow(i,2); j < this->array_size(); j += i) {
                        this->at(j) = false; 
                        this->curr_thds.at(id) = j;
                    }
                    this->curr_thds.at(id) = this->array_size();
                }
            } else
            {
                usleep(0.001);
            }
            
        }
    };

    // Start Looping through numbers
    for(unsigned int i = 2; i <= sqrt(this->array_size()); i++) {
        // If the number is a prime number up to this point
        if(this->at(i)) {
            // std::cout << "Checking " << i << "\n";
            // This loop will check if our main thread is behind our worker threads
            bool min = false;
            do {
                min = false;
                for(auto min_thd: this->curr_thds) {
                    if(min_thd <= i) {
                        min = false;
                        usleep(0.0001);
                    }
                }
            } while(min);

            // If a thread has not taken the old prime, wait
            while(this->current_prime != 1)
                usleep(0.00001);

            // Set the new prime for threads to work on
            this->current_prime = i;
            // std::cout << "Set new prime\n";
            // Spawn the number of threads
            if(this->thread_pool.size() < this->num_threads) {
                this->curr_thds.push_back(i);
                std::thread thd = std::thread(worker_func, this->curr_thds.size() - 1);
                this->thread_pool.push_back(std::move(thd));
            }
        }
    }
    // Tell the threads to stop working
    // std::cout << "\nStop Marking!!\n";
    this->marking = false;

    for(auto& thd : this->thread_pool ) {
        if(thd.joinable()) {
            thd.join();
        }
    }
}
