#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <chrono>
#include <ctime>

void remove_multiples(std::vector<unsigned>& nums, std::vector<unsigned>& primes);

std::vector<unsigned> create_wheel(std::vector<unsigned>& starter_primes);
void update_wheel(std::vector<unsigned>& wheel, unsigned new_prime);
std::vector<unsigned> find_incs(std::vector<unsigned>& starter_primes);

std::vector<unsigned> wheel_find_primes(std::vector<unsigned>& starter_primes, unsigned max_value);
std::vector<unsigned> wheel_sieve(std::vector<unsigned>& starter_primes, unsigned max_value);