//
// Created by jdhales on 7/15/21.
//

//#include "Primes.h"
#include <omp.h>
#include <vector>
#include <math.h>
#include <iostream>
#include <chrono>
#include <sstream>

/**
 * Sieve between the min and max indices in sieve.
 * @param sieve
 * @param min
 * @param max
 * @param prime
 */
void sieve_between(std::vector<bool>& sieve, uint64_t min, uint64_t max, uint64_t prime) {
    uint64_t start_index;
    //std::cout << "\nmin: " << min << std::endl;
    //std::cout << "\nprime: " << prime << std::endl;
    if(min == 0) {
        start_index = ((prime - 3) / 2) + (prime * (prime / 2));
    }
    else {
        uint64_t start_num = (min * 2) + 3;
        //std::cout << "start num: " << start_num << std::endl;
        uint64_t quotient = start_num / (prime * 2);
        //std::cout << "\tquotient: " << quotient << std::endl;
        uint64_t remainder = start_num % (prime * 2);
        //std::cout << "\tremainder: " << remainder << std::endl;
        start_index = (quotient * (prime * 2)) + prime;
        //std::cout << "\tprime * remainder: " << start_index << std::endl;
        if(remainder > prime) {
            start_index += (prime * 2);
        }
        //std::cout << "\tnext dividend: " << start_index << std::endl;
        start_index = (start_index - 3) / 2;
    }

    //std::cout << "next dividend: " << (start_index * 2) + 3 << std::endl;

    //if(prime == 19) {
    //    std::cout << "prime: " << prime << std::endl;
    //    std::cout << min << " - " << start_index << " - " << max << std::endl;
    //}

    for(uint64_t m = start_index; m < max; m += prime) {
        sieve[m] = false;
    }
}

uint64_t overestimate_pi(uint64_t max_value) {
    return uint64_t(double(max_value) / (log(max_value) - 1.1));
}

std::vector<uint64_t> gen_primes(uint64_t max_value) {
    uint64_t primes_array_size = overestimate_pi(max_value);
    //std::cout << "array size:" << primes_array_size << std::endl;
    std::vector<uint64_t> primes(primes_array_size, 0);
    primes[0] = 2;
    uint64_t p_index = 1;
    uint64_t root_max_value = uint64_t(ceil(sqrt(max_value)));

    // 0, 1, 2, 3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, ...
    // 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, ...
    uint64_t sieve_size = (max_value / 2) - 1;
    std::vector<bool> sieve(sieve_size, true);

    uint64_t prime;
    for (uint64_t i = 0; i < root_max_value; ++i) {
        if (sieve[i]) {
            prime = (i * 2) + 3;
            //std::cout << "new prime location:" << p_index << std::endl;
            primes[p_index] = prime;
            //std::cout << "new prime:" << prime << std::endl;
            ++p_index;
            for (uint64_t m = i + (prime * (prime / 2)); m < sieve_size; m += prime) {
                sieve[m] = false;
            }
        }
    }

    for (uint64_t i = root_max_value; i < sieve_size; ++i) {
        if (sieve[i]) {
            primes[p_index] = (i * 2) + 3;
            ++p_index;
        }
    }

    primes.resize(p_index);

    return primes;
}

void print_primes(uint64_t max_value) {
    uint64_t root_max_value = uint64_t(ceil(sqrt(max_value)));

    // 0, 1, 2, 3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, ...
    // 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, ...
    uint64_t sieve_size = (max_value / 2) - 1;
    std::vector<bool> sieve(sieve_size, true);
    std::cout << 2 << '\n';
    uint64_t prime;
    for (uint64_t i = 0; i < root_max_value; ++i) {
        if (sieve[i]) {
            prime = (i * 2) + 3;
            std::cout << prime << '\n';
            for (uint64_t m = i + (prime * (prime / 2)); m < sieve_size; m += prime) {
                sieve[m] = false;
            }
        }
    }

    for (uint64_t i = root_max_value; i < sieve_size; ++i) {
        if (sieve[i]) {
            std::cout << (i * 2) + 3 << '\n';
        }
    }

    std::cout << std::flush;

    return;
}

std::vector<uint64_t> gen_primes_parallel(uint64_t max_value) {
    uint64_t primes_array_size = overestimate_pi(max_value);
    //std::cout << "array size: " << primes_array_size << std::endl;
    std::vector<uint64_t> primes(primes_array_size, 0);
    primes[0] = 2;
    uint64_t p_index = 1;
    //std::cout << p_index << std::endl;
    uint64_t root_max_value = uint64_t(floor(sqrt(max_value)));

    // 0, 1, 2, 3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, ...
    // 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, ...
    uint64_t sieve_size = (max_value / 2) - 1;
    std::vector<bool> sieve(sieve_size, true);

    uint divisions = 4;
    //std::cout << sieve_size << std::endl;
    double partition_size = sieve_size / double(divisions);
    uint64_t lower_bounds[divisions];
    uint64_t upper_bounds[divisions];

    //std::cout << partition_size << std::endl;
    //return primes;

    double index = 0;
    for(uint64_t i = 0; i < divisions; ++i) {
        lower_bounds[i] = uint64_t(round(index));
        index += partition_size;
        upper_bounds[i] = uint64_t(round(index));

        //std::cout << lower_bounds[i] << ", " << upper_bounds[i] << std::endl;
    }

    if(upper_bounds[divisions - 1] != sieve_size) {
        upper_bounds[divisions - 1] = sieve_size;
    }

    uint64_t prime = 0;
    for (uint64_t i = 0; i < root_max_value; ++i) {
        if (sieve[i]) {
            prime = (i * 2) + 3;
            //std::cout << "new prime location:" << p_index << std::endl;
            //std::cout << primes.size() << std::endl;
            primes[p_index] = prime;
            //std::cout << "new prime: " << prime << std::endl;
            ++p_index;
            //std::cout << p_index << std::endl;
            //std::cout << "prime: " << prime << std::endl;
#pragma omp parallel for default(none) shared(lower_bounds, upper_bounds, divisions, sieve, prime, std::cout)
            for(uint d = 0; d < divisions; ++d) {
                //std::cout << d << std::endl;
                sieve_between(sieve, lower_bounds[d], upper_bounds[d], prime);
            }
        }
    }

    //std::cout << "past parallel part" << std::endl;

    //for(uint64_t i = 0; i < sieve.size(); ++i) {
    //    if(sieve[i]) {
    //        std::cout << i << " true" << std::endl;
    //    }
    //    else {
    //        std::cout << i << " false" << std::endl;
    //    }
    //}

    for (uint64_t i = root_max_value; i < sieve_size; ++i) {
        if (sieve[i]) {
            primes[p_index] = (i * 2) + 3;
            ++p_index;
            //std::cout << p_index << std::endl;
        }
    }

    primes.resize(p_index);

    return primes;
}

std::vector<uint64_t> gen_primes_wheeled(uint64_t max_value) {
    uint64_t wheel[8] = {4, 2, 4, 2, 4, 6, 2, 6};
    uint64_t wheel_i = 0;
    uint64_t sum[8] = {4, 6, 10, 12, 16, 22, 24, 30};
    uint64_t turns = 0;
    //uint64_t width = 30;
    uint64_t primes_array_size = overestimate_pi(max_value);

    std::vector<uint64_t> primes(primes_array_size, 0);

    return primes;
    primes[0] = 2;
    primes[1] = 3;
    primes[2] = 5;
    uint64_t p_index = 3;

    uint64_t root_max_value = uint64_t(floor(sqrt(max_value)));

    uint64_t sieve_size = (max_value * 0.3) - 3;
    std::vector<bool> sieve(sieve_size, true);

    uint64_t prime;
    for (uint64_t i = 0; i < root_max_value; ++i) {
        if (sieve[i]) {
            prime = turns * 30 + wheel[wheel_i];
            primes[p_index] = prime;
            prime = (i * 2) + 3;
            //std::cout << "new prime location:" << p_index << std::endl;
            primes[p_index] = prime;
            //std::cout << "new prime:" << prime << std::endl;
            ++p_index;
            for (uint64_t m = i + (prime * (prime / 2)); m < sieve_size; m += prime) {
                sieve[m] = false;
            }
        }
    }


}

// Should include best guess of all prime numbers between [0, max_value) and sorted from smallest to largest.
// Returns the first non-prime number OR 0 if all are prime.
uint64_t all_prime(std::vector<uint64_t> primes) {
    uint64_t max_root = 0;
    for(uint64_t p = 0; p < primes.size(); ++p) {
        //std::cout << primes[p] << std::endl;
        max_root = uint64_t(ceil(sqrt(primes[p])));
        for(uint64_t t = 0; primes[t] < max_root; ++t) {
            if(primes[p] % primes[t] == 0) {
                return primes[p];
            }
        }
    }

    return 0;
}

int main(int argc, char* argv[]) {
    //for(uint64_t i = 10; i < 10000000000000000000; i *= 10) {
    //    std::cout << "10^" <<log(i) / log(10) << ": " << uint64_t(double(i) / (log(i) - 1.1)) << std::endl;
    //}

    //std::cout << "Running ..." << std::endl;
    uint64_t max_value;
    std::istringstream iss(argv[1]);
    iss >> max_value;
    //auto start_gen = std::chrono::system_clock::now();
    print_primes(max_value);
    //auto end_gen = std::chrono::system_clock::now();
    //std::chrono::duration<double> elapsed = end_gen - start_gen;

    //std::cout << "serial: " << elapsed.count() << std::endl;
    //std::cout << "\tsize: " << primes.size() << std::endl;

    //std::cout << all_prime(primes) << std::endl;

    /*
    start_gen = std::chrono::system_clock::now();
    primes = gen_primes_parallel(max_value);
    end_gen = std::chrono::system_clock::now();
    elapsed = end_gen - start_gen;

    std::cout << "parallel: " << elapsed.count() << std::endl;
    std::cout << "\tsize: " << primes.size() << std::endl;
    */
    //for(uint64_t i = 0; i < primes.size(); ++i) {
    //    std::cout << primes[i] << std::endl;
    //}

    //std::cout << all_prime(primes) << std::endl;

    //uint64_t result = all_prime(primes);
    //std::cout << "total number of primes: " << primes.size() << std::endl;
    //std::cout << "all_prime() result: " << result << std::endl;
    //auto end_sieve = std::chrono::system_clock::now();
    //std::chrono::duration<double> elapsed_sieve = end_sieve - start_sieve;
    //std::cout << elapsed_sieve.count() << std::endl;


    //for(size_t& p : primes) {
    //    std::cout << p << std::endl;
    //}
    //std::cout << "done" << std::endl;
    return 0;
}