#include "primes.hpp"

void remove_multiples(std::vector<unsigned>& nums, std::vector<unsigned>& primes) {
    // Sieve of Eratosthenes
    for(unsigned& prime : primes) {
        for(unsigned i = prime; i < nums.size(); i += prime) {
            nums[i] = 0;
        }
    }
}

std::vector<unsigned> create_wheel(std::vector<unsigned>& starter_primes) {
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

std::vector<unsigned> find_incs(std::vector<unsigned>& starter_primes) {
    std::vector<unsigned> wheel = create_wheel(starter_primes);

    for(unsigned& num : wheel) {
        std::cout << num << std::endl;
    }

    std::vector<unsigned> incs;

    // Find the prime after the starter primes
    unsigned wheel_prime = 0;
    for(unsigned i = starter_primes.back() + 1; i < wheel.size(); ++i) {
        if(wheel[i] != 0) {
            wheel_prime = wheel[i];
            break;
        }
    }

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

    return incs;
}

bool is_prime(unsigned num, std::vector<unsigned>& previous_primes) {
    double max_test_value = ceil(sqrt(double(num)));

    for(unsigned& prime : previous_primes) {
        if(prime <= max_test_value) {
            if(num % prime == 0) {
                return false;
            }
        }
        else {
            return true;
        }
    }
}

std::vector<unsigned> wheel_find_primes(std::vector<unsigned>& starter_primes, unsigned max_value) {
    std::sort(starter_primes.begin(), starter_primes.end());
    std::vector<unsigned> incs = find_incs(starter_primes);

    std::vector<unsigned> new_primes;

    // Find next prime after last starter_prime
    for(unsigned i = starter_primes.back() + 2;; i += 2) {
        if(is_prime(i, starter_primes)) {
            new_primes.push_back(i);
            break;
        }
    }

    // We're now in the right place to start the wheel
    unsigned i = 0;
    for(unsigned next = new_primes.back();;) {
        next += incs[i];
        i += 1;
        if(i >= incs.size()) {
            i = 0;
        }

        if(next > max_value) {
            break;
        }

        if(is_prime(next, new_primes)) {
            new_primes.push_back(next);
        }
    }

    std::vector<unsigned> primes = starter_primes;
    primes.insert(primes.end(), new_primes.begin(), new_primes.end());

    return primes;
}

unsigned all_prime(std::vector<unsigned>& primes) {
    for(unsigned& prime : primes) {
        if(prime % 2 == 0 && prime != 2) {
            return prime;
        }
        else {
            for(unsigned i = 3; i < double(prime) / 2; i += 2) {
                if(prime % i == 0 && prime != i) {
                    return prime;
                }
            }
        }
    }

    return 0;
}

std::vector<unsigned> update_incs(std::vector<unsigned>& previous_incs, std::vector<unsigned>& primes) {
    unsigned wheel_size = 1;
    for(unsigned& prime : primes) {
        wheel_size *= prime;
    }

    std::vector<unsigned> new_wheel(wheel_size, 0);
    for(unsigned i = 1; i < new_wheel.size(); ++i) {
        new_wheel[i] = i;
    }
    /*
    if(previous_wheel.size() == 0 && primes.size() == 1 && primes[0] == 2) {
        new_wheel.push_back(2);
        return new_wheel;
    }
    */

}

std::vector<unsigned> sieve(unsigned max_value) {
    std::vector<bool> nums(max_value, true);

    nums[0] = false;
    nums[1] = false;

    std::vector<unsigned> primes;

    // double sqrt_max_value = ceil(sqrt(max_value));
    for(unsigned i = 2; i < max_value; ++i) {
        if(nums[i]) {
            primes.push_back(i);

            for(unsigned m = i * i; m < max_value; m += i) {
                nums[m] = false;
            }
        }
    }

    return primes;
}

std::vector<unsigned> sieve2(unsigned max_value) {
    std::vector<unsigned> primes;
    if(max_value == 0) {
        return primes;
    }

    std::vector<bool> nums(max_value, true);
    nums[0] = false;
    if(max_value == 1) {
        return primes;
    }
    nums[1] = false;
    for(unsigned i = 2; i < max_value; ++i) {
        if(nums[i]) {
            primes.push_back(i);
            if(i == 2) {
                for(unsigned m = i * i; m < max_value; m += i) {
                    nums[m] = false;
                }
            }
            else {
                for(unsigned m = i * i; m < max_value; m += 2 * i) {
                    nums[m] = false;
                }
            }
        }
    }

    return primes;
}

std::vector<unsigned> sieve_p(unsigned max_value) {
    std::vector<unsigned> primes;
    if(max_value == 0) {
        return primes;
    }

    std::vector<bool> nums(max_value, true);
    nums[0] = false;
    if(max_value == 1) {
        return primes;
    }


}

