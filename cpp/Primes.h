//
// Created by jdhales on 7/15/21.
//

#ifndef CPP_PRIMES_H
#define CPP_PRIMES_H

#include <vector>
#include <list>
#include <iostream>
#include <math.h>
#include <omp.h>

class Primes {
private:
    static std::vector<bool> get_2_sieve(uint64_t max_value);
    static std::list<uint64_t> convert_2_sieve(const std::vector<bool>& sieve);
public:
    Primes();

    static void remove_multiples(std::vector<unsigned int>& nums, std::vector<unsigned int>& primes);
    static std::vector<unsigned int> create_wheel(std::vector<unsigned int>& starter_primes);
    void find_incs();

    static uint64_t all_prime(std::vector<uint64_t> primes);

    static uint64_t overestimate_pi_func(uint64_t max_value);

    static std::list<uint64_t> sieve2(uint64_t max_value);

    static std::list<uint64_t> sieve_between(const std::list<uint64_t>& starter_primes, uint64_t start, uint64_t end);
    static std::vector<bool> get_2_sieve_between(std::list<uint64_t> starter_primes, uint64_t start, uint64_t end);
    static void convert_2_sieve_between(std::list<uint64_t>& primes, const std::vector<bool>& sieve, uint64_t start);
    static std::list<uint64_t> sieve_p(uint64_t max_value);

    static std::vector<uint64_t> gen_primes(uint64_t max_value);
};


#endif //CPP_PRIMES_H
