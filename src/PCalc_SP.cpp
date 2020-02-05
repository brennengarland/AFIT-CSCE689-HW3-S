#include "PCalc_SP.h"
#include <unistd.h>
#include <math.h>
#include <iostream>


PCalc_SP::PCalc_SP(unsigned int array_size): PCalc(array_size) {
}


void PCalc_SP::markNonPrimes()  {
    // std::cout << "\nArray Size: " << this->array_size() << "\n";
    for(int i = 2; i <= sqrt(this->array_size()); i++) {
        if(this->at(i)) {
            for(int j = pow(i,2); j <= this->array_size(); j += i) {
                (this->at(j)) = false; 
            }
        }
    }
}
