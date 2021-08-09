//
// Created by jdhales on 7/15/21.
//

#include "Primes.h"

Primes::Primes() = default;// {
    /*if(starter_primes == nullptr || starter_primes->size() < 4) {
        primes = std::vector<unsigned int>{2, 3, 5, 7};
    }
    else {
        primes = *starter_primes;
    }*/
    //primes = std::vector<unsigned int>();
    //incs = std::vector<unsigned int>();
    //find_incs();
//}

void Primes::remove_multiples(std::vector<unsigned int>& nums, std::vector<unsigned int>& primes) {
    // Sieve of Eratosthenes
    for(unsigned& prime : primes) {
        for(unsigned i = prime; i < nums.size(); i += prime) {
            nums[i] = 0;
        }
    }
}

std::vector<unsigned int> Primes::create_wheel(std::vector<unsigned int>& starter_primes) {
    unsigned wheel_size = 1;
    for(unsigned& prime : starter_primes) {
        wheel_size *= prime;
    }

    // Create the basic wheel
    std::vector<unsigned> wheel(wheel_size, 0);
    for(unsigned i = 1; i < wheel.size(); ++i) {
        wheel[i] = i;
    }

    // Sieve the wheel
    remove_multiples(wheel, starter_primes);

    return wheel;
}
/*
void Primes::find_incs() {
    std::vector<unsigned int> wheel = create_wheel(primes);

    for(unsigned& num : wheel) {
        std::cout << num << std::endl;
    }

    // Find the prime after the starter primes
    unsigned wheel_prime = 0;
    for(unsigned i = primes.back() + 1; i < wheel.size(); ++i) {
        if(wheel[i] != 0) {
            wheel_prime = wheel[i];
            break;
        }
    }

    incs.clear();

    // Get the increments
    unsigned inc = 1;
    unsigned i = wheel_prime;
    for(;;) {
        ++i;
        if (i >= wheel.size()) {
            i = 0;
        }

        if (wheel[i] != 0) {
            incs.push_back(inc);
            inc = 1;
        } else {
            ++inc;
        }

        if (i == wheel_prime) {
            break;
        }
    }
}
*/
/**
 * Finds the sieve up to max_value
 * Returned sieve will only include odd numbers
 * @param max_value
 * @return
 */
std::vector<bool> Primes::get_2_sieve(uint64_t max_value) {
    // 1, 2, 3, 4, 5, 6,  7,  8,  9,  10, 11, 12, 13, 14, 15
    // 0, 1, 2, 3, 4, 5,  6,  7,  8,   9, 10, 11, 12, 13, 14
    // 1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29
    std::vector<bool> nums(max_value / 2, true);
    std::cout << nums.size() << std::endl;
    if(max_value >= 2) {
        nums[0] = false;
    }
    if(max_value <= 3) {
        return nums;
    }

    auto root = uint64_t(ceil(sqrt(max_value)));
    uint64_t p;
    for(uint64_t i = 0; i < nums.size(); ++i) {
        if(nums[i]) {
            p = (i * 2) + 1;
            if(p <= root) {
                for(uint64_t m = i + p; m < max_value; m += p) {
                    nums[m] = false;
                }
            }
        }
    }

    return nums;
}

std::list<uint64_t> Primes::convert_2_sieve(const std::vector<bool>& sieve) {
    std::list<uint64_t> primes;
    std::cout << "converting 2 sieve" << std::endl;
    std::cout << sieve.size() << std::endl;
    primes.push_back(2);
    std::cout << 2 << std::endl;
    std::cout << "size of primes: " << primes.size() << std::endl;
    for(uint64_t i = 1; i < sieve.size(); ++i) {
        if(sieve[i]) {
            std::cout << (i * 2) + 1 << std::endl;
            primes.push_back((i * 2) + 1);
            std::cout << "size of primes: " << primes.size() << std::endl;
        }
    }

    return primes;
}

std::list<uint64_t> Primes::sieve2(uint64_t max_value) {
    return Primes::convert_2_sieve(Primes::get_2_sieve(max_value));
}

std::list<uint64_t> Primes::sieve_between(const std::list<uint64_t>& starter_primes, uint64_t start, uint64_t end) {
    std::cout << "start: " << start << std::endl;
    std::cout << "finish: " << end << std::endl;

    std::list<uint64_t> primes;

    std::vector<bool> nums(end - start, true);

    uint64_t shift = start % 2;
    uint64_t i;
    // Get 2 out of the way
    for(i = shift; i < nums.size(); i += 2) {
        nums[i] = false;
    }

    // 3 - start % 3

    // Go through the rest of the primes
    uint64_t p_double;
    uint64_t start_index;
    uint64_t n;
    for(auto p = std::next(starter_primes.begin()); p != starter_primes.end(); ++p) {
        p_double = (*p * 2);
        start_index = p_double - ((start + *p) % p_double);
        if(start_index == p_double) {
            start_index = 0;
        }
        for(n = start_index; n < nums.size(); n += p_double) {
            nums[n] = false;
        }
    }

    uint64_t count = 0;
    for(i = (shift + 1) % 2; i < nums.size(); i += 2) {
        if(nums[i]) {
            primes.push_back(i + start);
            ++count;
        }
    }

    std::cout << "number of primes: " << count << std::endl;

    return primes;
}
/**
 * Includes the start but not the end
 * @param starter_primes
 * @param start
 * @param end
 * @return
 */
std::vector<bool> Primes::get_2_sieve_between(std::list<uint64_t> starter_primes, uint64_t start, uint64_t end) {
    start = start + ((start + 1) % 2); // Make start odd
    end = end - (end % 2); // Make end even
    // These two changes do not affect the range of numbers tested
    std::cout << "start: " << start << std::endl;
    std::cout << "end: " << end << std::endl;

    uint64_t distance = end - start;
    std::vector<bool> nums(distance / 2, true);

    uint64_t p_double;
    uint64_t start_index;
    uint64_t n;
    for(auto p = std::next(starter_primes.begin()); p != starter_primes.end(); ++p) {
        p_double = *p * 2;
        start_index = (p_double - ((start + *p) % p_double)) / 2;
        if(start_index == *p) {
            start_index = 0;
        }
        for(n = start_index; n < nums.size(); n += *p) {
            nums[n] = false;
        }
    }

    // Index for 3: start % 3
    // Index for 5: (start * 2) % 5
    // Index for 7:

    return nums;
}

void Primes::convert_2_sieve_between(std::list<uint64_t>& primes, const std::vector<bool>& sieve, uint64_t start) {
    start = start + ((start + 1) % 2); // Make start odd in the same way that Primes::get_2_sieve_between does
    for(uint64_t i = 0; i < sieve.size(); ++i) {
        if(sieve[i]) {
            primes.push_back(start + (2 * i));
        }
    }
}

uint64_t Primes::overestimate_pi_func(uint64_t max_value) {
    return uint64_t(double(max_value) / (log(max_value) - 2));
}

/*
 * 1 array of uint64_t of sufficient size which holds all of the prime numbers (use pi function)
 * 1 vector of bool
 */

std::list<uint64_t> Primes::sieve_p(uint64_t max_value) {
    uint64_t array_size = Primes::overestimate_pi_func(max_value);
    //auto* primes = new uint64_t[array_size];
    auto root_max_value = uint64_t(ceil(sqrt(max_value)));


    std::list<uint64_t> starter_primes = Primes::sieve2(root_max_value);
    //starter_primes.reserve(Primes::overestimate_pi_func(max_value));
    //Primes::sieve2(starter_primes, root_max_value);

    unsigned int divisions = 4;
    double division_size = double(max_value - root_max_value) / double(divisions);
    std::list<uint64_t> primes;
    primes.insert(primes.end(), starter_primes.begin(), starter_primes.end());
    omp_set_dynamic(0);
#pragma omp parallel for ordered default(none) shared(max_value, root_max_value, divisions, division_size, starter_primes, primes, std::cout)
    for(unsigned int i = 0; i < divisions; ++i) {
        //unsigned id = omp_get_thread_num();
        auto start = root_max_value + uint64_t(ceil(i * division_size));
        auto finish = root_max_value + uint64_t(ceil((i + 1) * division_size));
        std::cout << i << " starting" << std::endl;

        std::list<uint64_t> partial = Primes::sieve_between(starter_primes, start, finish);
        std::cout << i << " finished" << std::endl;
#pragma omp ordered
        {
            primes.insert(primes.end(), partial.begin(), partial.end());
            //partial.erase(partial.begin(), partial.end());
        }
    }

    return primes;
}

/**
 * Determines if all stored numbers are prime.
 * @return The first failing candidate or 0 if all are prime.
 */
uint64_t Primes::all_prime(std::vector<uint64_t> primes) {
    if(primes.empty()) {
        return 0;
    }

    if(primes[0] == 1) {
        return 1;
    }
    uint64_t root;
    int i;
    for(uint64_t& prime : primes) {
        root = uint64_t(ceil(sqrt(prime)));
        for(i = 0; primes[i] < root; ++i) {
            if(prime % primes[i] == 0) {
                return prime;
            }
        }
    }

    return 0;
}

std::vector<uint64_t> Primes::gen_primes(uint64_t max_value) {
    uint64_t primes_array_size = uint64_t(double(max_value) / (log(max_value) - 2));
    //std::cout << "array size:" << primes_array_size << std::endl;
    std::vector<uint64_t> primes(primes_array_size, 0);
    primes[0] = 2;
    uint64_t p_index = 1;
    uint64_t root_max_value = uint64_t(ceil(sqrt(max_value)));

    // 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, ...
    uint64_t sieve_size = (max_value / 2) - 1;
    std::vector<bool> sieve(sieve_size, true);
    uint64_t prime = 0;
    for(uint64_t i = 0; i < root_max_value; ++i) {
        if(sieve[i]) {
            prime = (i * 2) + 3;
            //std::cout << "new prime location:" << p_index << std::endl;
            primes[p_index] = prime;
            //std::cout << "new prime:" << prime << std::endl;
            ++p_index;
            for(uint64_t m = i + (prime * (prime / 2)); m < sieve_size; m += prime) {
                sieve[m] = false;
            }
        }
    }

    for(uint64_t i = root_max_value; i < sieve_size; ++i) {
        if(sieve[i]) {
            primes[p_index] = (i * 2) + 3;
            ++p_index;
        }
    }

    primes.resize(p_index);

    return primes;

    // Find the primes less than the square root of the maximum value
    /*uint64_t small_sieve_size = (root_max_value / 2) - 1;
    uint64_t small_sieve_size_root = uint64_t(ceil(sqrt(small_sieve_size)));
    std::vector<bool> small_sieve(small_sieve_size, true);
    uint64_t prime = 0;
    for(uint64_t i = 0; i < small_sieve_size; ++i) {
        if(small_sieve[i]) {
            prime = (i * 2) + 3;
            primes[p_index] = prime;
            if(i < small_sieve_size_root) {
                for(uint64_t s = i + prime; s < small_sieve_size; s += prime) {
                    small_sieve[s] = false;
                }
            }
        }
    }*/
}




