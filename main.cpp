#include <iostream>
#include <sstream>
#include "primes.h"

int main(int argc, char* argv[]) {
    if(argc == 2) {
        std::stringstream input(argv[1]);
        size_t num;
        input >> num;
        primes::GenerateAndSavePrimesThreaded(num);
    }
    return 0;
}
