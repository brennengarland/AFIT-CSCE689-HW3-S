#include "PCalc_T.h"
#include <unistd.h>
#include <math.h>
#include <thread>
#include <iostream>

PCalc_T::PCalc_T(unsigned int array_size, unsigned int num_threads): PCalc(array_size) {
    this->num_threads = num_threads;
}


void PCalc_T::markNonPrimes()  {
    std::cout << "Array Size: " << array_size() << "\n\n";
    for(int i = 2; i <= sqrt(this->array_size()); i++) {
        std::cout << "\nChecking [" << i << "]\n";
        if(this->at(i)) {
            bool min = true;
            do {
                min = true;
                for(auto min_thd: curr_thds) {
                    if(min_thd <= i) {
                        min = false;
                        usleep(100);
                        std::cout << "\tMin thread found: " << min_thd << " vs " << i << "\n";
                    }
                }
            } while(!min);
            std::cout << "\tCurrent Prime: " << current_prime << "\n";

            while(current_prime != 1) {usleep(0.0000000000001);}
            thd_lock.lock();
            current_prime = i;
            thd_lock.unlock();
            std::cout << "\tSet Prime to: " << current_prime << "\n";


            if(thread_pool.size() < num_threads) {
                std::cout << "\tThread Count: " << thread_pool.size() << "\n";
                std::cout << "\tMaking thread...\n";
                curr_thds.push_back(i);
                std::cout << "\tCurrent Thread count: " << curr_thds.size() << "\n";

                std::thread thd = std::thread([=]{threadNonPrimes(curr_thds.size() - 1);});

                thread_pool.push_back(std::move(thd));

            }
        }
    }
    
    current_prime = array_size();
        // std::cout << "\nJoining threads\n";
    for(auto& thd : thread_pool ) {
        if(thd.joinable()) {
            thd.join();
            // std::cout << "\tThread Joined!\n";
        }
    }
}

void PCalc_T::threadNonPrimes(int id) {
    // std::cout << "\nNew Thread " << id << "\n";
    int i;
    while(current_prime != array_size()) {
        while(current_prime == 1) {
            std::cout << "Thread " << id << " waiting on main..\n";
            usleep(100000);
        }
        thd_lock.lock();
        i = current_prime;
        current_prime = 1;
        thd_lock.unlock();
        
        for(int j = pow(i,2); j <= this->array_size(); j += i) {
            (this->at(j)) = false; 
            curr_thds.at(id) = j;
        }
        curr_thds.at(id) = array_size();

    }
}


            