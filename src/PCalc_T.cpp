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
    for(int i = 2; i <= sqrt(this->array_size()); i++) {
        if(this->at(i)) {
            // std::cout << "Checking " << i << "\n";
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

            while(current_prime != 1) {
                usleep(0.000001);
            }
            
            current_prime = i;


            if(thread_pool.size() < num_threads) {
                curr_thds.push_back(i);
                std::cout << "Creating thread " << curr_thds.size()-1 << "\n";
                // std::cout << "Thread " << curr_thds.size()-1 << " made\n";
                std::thread thd = std::thread([=]{threadNonPrimes(curr_thds.size() - 1);});
                thread_pool.push_back(std::move(thd));
                // std::cout << "Thread made!\n";
            }
            
        }
    }
    
    current_prime = array_size();
    for(auto& thd : thread_pool ) {
        if(thd.joinable()) {
            thd.join();
            // std::cout << "\tThread Joined!\n";
        }
    }
}

void PCalc_T::threadNonPrimes(int id) {
    int i;
    // std::cout << "Thread " << id << " starting\n";
    while(true) {
        while(current_prime == 1) {
            usleep(0.000001);
        }
        // std::cout << "Thread " << id << " ready to read prime\n";

        if(current_prime != array_size()) {
            std::lock_guard<std::mutex> lock(thd_lock);
            i = current_prime;
            current_prime = 1;
        } else {
            break;
        }

        // std::cout << "Thread " << id << " calculating " << i << "\n";
        for(int j = pow(i,2); j <= this->array_size(); j += i) {
            (this->at(j)) = false; 
            curr_thds.at(id) = j;
        }

        // std::cout << "Thread " << id << " finished work on " << i << "\n";
        curr_thds.at(id) = array_size();
    }
}


            