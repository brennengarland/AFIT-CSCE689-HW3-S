#include "PCalc_T.h"
#include <unistd.h>
#include <math.h>


PCalc_T::PCalc_T(unsigned int array_size, unsigned int num_threads): PCalc(array_size) {
}


void PCalc_T::markNonPrimes()  {

    for(int i = 2; i <= sqrt(this->array_size()); i++) {
        if(this->at(i)) {
            for(int j = pow(i,2); j <= this->array_size(); j += i) {
                // this[j] = false;
            }
        }
    }
}
