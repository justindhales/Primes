//
// Created by jdhales on 7/15/21.
//

// #include "Primes.h"
#include <math.h>
#include <omp.h>

#include <chrono>
#include <iostream>
#include <sstream>
#include <vector>

/**
 * Sieve between the min and max indices in sieve.
 * @param sieve
 * @param min
 * @param max
 * @param prime
 */
void sieve_between(std::vector<bool>& sieve, uint64_t min, uint64_t max,
                   uint64_t prime) {
  uint64_t start_index;
  // std::cout << "\nmin: " << min << std::endl;
  // std::cout << "\nprime: " << prime << std::endl;
  if (min == 0) {
    start_index = ((prime - 3) / 2) + (prime * (prime / 2));
  } else {
    uint64_t start_num = (min * 2) + 3;
    // std::cout << "start num: " << start_num << std::endl;
    uint64_t quotient = start_num / (prime * 2);
    // std::cout << "\tquotient: " << quotient << std::endl;
    uint64_t remainder = start_num % (prime * 2);
    // std::cout << "\tremainder: " << remainder << std::endl;
    start_index = (quotient * (prime * 2)) + prime;
    // std::cout << "\tprime * remainder: " << start_index << std::endl;
    if (remainder > prime) {
      start_index += (prime * 2);
    }
    // std::cout << "\tnext dividend: " << start_index << std::endl;
    start_index = (start_index - 3) / 2;
  }

  // std::cout << "next dividend: " << (start_index * 2) + 3 << std::endl;

  // if(prime == 19) {
  //     std::cout << "prime: " << prime << std::endl;
  //     std::cout << min << " - " << start_index << " - " << max << std::endl;
  // }

  for (uint64_t m = start_index; m < max; m += prime) {
    sieve[m] = false;
  }
}

uint64_t overestimate_pi(uint64_t max_value) {
  return uint64_t(double(max_value) / (log(max_value) - 1.1));
}

std::vector<uint64_t> gen_primes(uint64_t max_value) {
  uint64_t primes_array_size = overestimate_pi(max_value);
  // std::cout << "array size:" << primes_array_size << std::endl;
  std::vector<uint64_t> primes(primes_array_size, 0);
  primes[0] = 2;
  uint64_t p_index = 1;
  uint64_t root_max_value = uint64_t(ceil(sqrt(max_value)));

  // 0, 1, 2, 3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
  // 20, 21, 22, 23, ... 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31,
  // 33, 35, 37, 39, 41, 43, 45, 47, 49, ...
  uint64_t sieve_size = (max_value / 2) - 1;
  std::vector<bool> sieve(sieve_size, true);

  uint64_t prime;
  for (uint64_t i = 0; i < root_max_value; ++i) {
    if (sieve[i]) {
      prime = (i * 2) + 3;
      // std::cout << "new prime location:" << p_index << std::endl;
      primes[p_index] = prime;
      // std::cout << "new prime:" << prime << std::endl;
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

void print_primes_bitmap(uint64_t max_value) {
  uint64_t max_value_root_index = (uint64_t(ceil(sqrt(max_value))) - 3) / 2;

  // 0, 1, 2, 3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
  // 20, 21, 22, 23, ... 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31,
  // 33, 35, 37, 39, 41, 43, 45, 47, 49, ...
  uint64_t sieve_size = ((max_value - 3) / 2) + 1;
  uint8_t* sieve = (uint8_t*)calloc((sieve_size / 8) + 1, sizeof(uint8_t));
  uint8_t mask = 1;
  std::cout << "2\n";
  uint64_t prime;
  uint64_t i;
  uint64_t m;
  for (i = 0; i <= max_value_root_index; ++i) {
    if ((sieve[i / 8] & mask << (i % 8)) == 0) {
      prime = (i * 2) + 3;
      std::cout << prime << '\n';
      // std::cout << ((prime * prime) / 2) - 1 << '\n';
      for (m = ((prime * prime) / 2) - 1; m < sieve_size; m += prime) {
        sieve[m / 8] |= mask << (m % 8);
        // printf("0x%02x\n", sieve[m / 8]);
        // std::cout << "byte " << m / 8 << ", bit " << m % 8 << std::endl;
      }
    }
  }

  for (i = max_value_root_index + 1; i < sieve_size; ++i) {
    if ((sieve[i / 8] & mask << (i % 8)) == 0) {
      std::cout << (i * 2) + 3 << '\n';
    }
  }

  std::cout << std::flush;
  free(sieve);
}

void print_primes_count_vector_sundaram(uint64_t max_value) {
  std::cout << "Sundaram's version" << std::endl;
  uint64_t sieve_size = ((max_value - 3) / 2) + 1;
  std::cout << "Sieve size: " << sieve_size << std::endl;
  std::vector<bool> sieve(sieve_size);

  uint64_t i;
  uint64_t m;
  uint64_t prime;
  uint64_t operations = 0;

  uint64_t max_value_root_index = (uint64_t(sqrt(max_value)) - 3) / 2 + 1;
  std::cout << "Max value root index: " << max_value_root_index << std::endl;
  for (i = 0; i < max_value_root_index; ++i) {
    prime = (i * 2) + 3;

    for (m = ((prime * prime) - 3) / 2; m < sieve_size; m += prime) {
      sieve[m] = true;
      ++operations;
    }
  }

  std::cout << "Total operations: " << operations << std::endl;

  uint64_t count = 1;
  for (i = 0; i < sieve_size; ++i) {
    if (!sieve[i]) {
      // std::cout << i * 2 + 1 << std::endl;
      ++count;
    }
  }

  std::cout << "Found " << count << " primes to " << max_value << std::endl;
}

void print_primes_count_vector(uint64_t max_value) {
  std::cout << "My version" << std::endl;
  uint64_t max_value_root_index = uint64_t((sqrt(max_value) - 3) / 2.0);

  // 0, 1, 2, 3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
  // 20, 21, 22, 23, ...
  // 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41,
  // 43, 45, 47, 49, ...
  uint64_t sieve_size = (max_value - 1) / 2;
  std::vector<bool> sieve(sieve_size);

  uint64_t prime;
  uint64_t i;
  uint64_t m;
  uint64_t operations = 0;
  uint64_t count = 1;
  for (i = 0; i <= max_value_root_index; ++i) {
    if (!sieve[i]) {
      ++count;
      prime = (i * 2) + 3;
      for (m = ((prime * prime) / 2) - 1; m < sieve_size; m += prime) {
        sieve[m] = true;
        ++operations;
      }
    }
  }

  std::cout << "Total operations: " << operations << std::endl;

  for (i = max_value_root_index + 1; i < sieve_size; ++i) {
    if (!sieve[i]) {
      ++count;
    }
  }

  std::cout << "Found " << count << " primes to " << max_value << std::endl;
}

void print_primes_count_vector_2(uint64_t max_value) {
  std::cout << "My version 2" << std::endl;
  uint64_t max_value_root_index = (uint64_t(ceil(sqrt(max_value))) - 3) / 2;

  // 0, 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17
  // 5, 7, 11, 13, 17, 19, 23, 25, 29, 31, 35, 37, 41, 43, 47, 49, 53, 55,
  uint64_t sieve_size = ((max_value - 5) / 2) + 1;
  std::vector<bool> sieve(sieve_size);

  uint64_t prime;
  uint64_t i;
  uint64_t m;
  uint64_t operations = 0;
  uint64_t count = 1;
  for (i = 0; i <= max_value_root_index; ++i) {
    if (!sieve[i]) {
      ++count;
      prime = (i * 2) + 3;
      for (m = ((prime * prime) / 2) - 1; m < sieve_size; m += prime) {
        sieve[m] = true;
        ++operations;
      }
    }
  }

  std::cout << "Total operations: " << operations << std::endl;

  for (i = max_value_root_index + 1; i < sieve_size; ++i) {
    if (!sieve[i]) {
      ++count;
    }
  }

  std::cout << "Found " << count << " primes to " << max_value << std::endl;
}

void print_primes_vector(uint64_t max_value) {
  uint64_t max_value_root_index = (uint64_t(ceil(sqrt(max_value))) - 3) / 2;

  // 0, 1, 2, 3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
  // 20, 21, 22, 23, ... 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31,
  // 33, 35, 37, 39, 41, 43, 45, 47, 49, ...
  uint64_t sieve_size = ((max_value - 3) / 2) + 1;
  std::vector<bool> sieve(sieve_size);
  std::cout << "2\n";
  uint64_t prime;
  uint64_t i;
  uint64_t m;
  for (i = 0; i <= max_value_root_index; ++i) {
    if (!sieve[i]) {
      prime = (i * 2) + 3;
      std::cout << prime << '\n';
      for (m = ((prime * prime) / 2) - 1; m < sieve_size; m += prime) {
        sieve[m] = true;
      }
    }
  }

  for (i = max_value_root_index + 1; i < sieve_size; ++i) {
    if (!sieve[i]) {
      std::cout << (i * 2) + 3 << '\n';
    }
  }

  std::cout << std::flush;
}

void print_primes2(uint64_t max_value) {
  uint64_t max_value_root_index = (uint64_t(ceil(sqrt(max_value))) - 3) / 2;

  // 0, 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
  // 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,  32,  33,  34,  35, 36,
  // 37,  38,  39,  40,  41... 5, 7, 11, 13, 17, 19, 23, 25, 29, 31, 35, 37, 41,
  // 43, 47, 49, 53, 55, 59, 61, 65, 67, 71, 73, 77, 79, 83, 85, 89, 91, 95, 97,
  // 101, 103, 107, 109, 113, 115, 117, 121,...
  uint64_t sieve_size = (max_value / 3) - 1;
  std::vector<bool> sieve(sieve_size, true);
  std::cout << "2\n3\n";
  uint64_t prime;

  for (uint64_t i = 0; i < max_value_root_index; ++i) {
    if (sieve[i]) {
      prime = (i * 3) - (i % 2) + 5;
      std::cout << prime << '\n';

      if (i % 2) {
        break;
      }
      for (uint64_t m = ((prime * prime) / 3) - 1; m < sieve_size; m += prime) {
        sieve[i] = false;
      }
    }
  }

  for (uint64_t i = max_value_root_index; i < sieve_size; ++i) {
    if (sieve[i]) {
      std::cout << (i * 3) - (i % 2) + 5 << '\n';
    }
  }
}

std::vector<uint64_t> gen_primes_parallel(uint64_t max_value) {
  uint64_t primes_array_size = overestimate_pi(max_value);
  // std::cout << "array size: " << primes_array_size << std::endl;
  std::vector<uint64_t> primes(primes_array_size, 0);
  primes[0] = 2;
  uint64_t p_index = 1;
  // std::cout << p_index << std::endl;
  uint64_t root_max_value = uint64_t(ceil(sqrt(max_value)));

  // 0, 1, 2, 3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
  // 20, 21, 22, 23, ... 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31,
  // 33, 35, 37, 39, 41, 43, 45, 47, 49, ...
  uint64_t sieve_size = (max_value / 2) - 1;
  std::vector<bool> sieve(sieve_size, true);

  uint divisions = 4;
  // std::cout << sieve_size << std::endl;
  double partition_size = sieve_size / double(divisions);
  uint64_t lower_bounds[divisions];
  uint64_t upper_bounds[divisions];

  // std::cout << partition_size << std::endl;
  // return primes;

  double index = 0;
  for (uint64_t i = 0; i < divisions; ++i) {
    lower_bounds[i] = uint64_t(round(index));
    index += partition_size;
    upper_bounds[i] = uint64_t(round(index));

    // std::cout << lower_bounds[i] << ", " << upper_bounds[i] << std::endl;
  }

  if (upper_bounds[divisions - 1] != sieve_size) {
    upper_bounds[divisions - 1] = sieve_size;
  }

  uint64_t prime = 0;
  for (uint64_t i = 0; i < root_max_value; ++i) {
    if (sieve[i]) {
      prime = (i * 2) + 3;
      // std::cout << "new prime location:" << p_index << std::endl;
      // std::cout << primes.size() << std::endl;
      primes[p_index] = prime;
      // std::cout << "new prime: " << prime << std::endl;
      ++p_index;
      // std::cout << p_index << std::endl;
      // std::cout << "prime: " << prime << std::endl;
#pragma omp parallel for default(none) \
    shared(lower_bounds, upper_bounds, divisions, sieve, prime, std::cout)
      for (uint d = 0; d < divisions; ++d) {
        // std::cout << d << std::endl;
        sieve_between(sieve, lower_bounds[d], upper_bounds[d], prime);
      }
    }
  }

  // std::cout << "past parallel part" << std::endl;

  // for(uint64_t i = 0; i < sieve.size(); ++i) {
  //     if(sieve[i]) {
  //         std::cout << i << " true" << std::endl;
  //     }
  //     else {
  //         std::cout << i << " false" << std::endl;
  //     }
  // }

  for (uint64_t i = root_max_value; i < sieve_size; ++i) {
    if (sieve[i]) {
      primes[p_index] = (i * 2) + 3;
      ++p_index;
      // std::cout << p_index << std::endl;
    }
  }

  primes.resize(p_index);

  return primes;
}

std::vector<uint64_t> gen_primes_wheeled(uint64_t max_value) {
  uint64_t wheel[8] = {4, 2, 4, 2, 4, 6, 2, 6};
  uint64_t wheel_i = 0;
  // uint64_t sum[8] = {4, 6, 10, 12, 16, 22, 24, 30};
  // uint64_t turns = 0;
  uint64_t primes_array_size = overestimate_pi(max_value);
  std::vector<uint64_t> primes(primes_array_size, 0);
  primes[0] = 2;
  primes[1] = 3;
  primes[2] = 5;
  uint64_t p_index = 3;

  uint64_t root_max_value = uint64_t(ceil(sqrt(max_value)));

  uint64_t sieve_size = (max_value / 2) - 1;
  std::vector<bool> sieve(sieve_size, true);

  // Remove multiples of 3 and 5
  uint64_t prime;
  for (uint64_t p = 0; p <= 1; ++p) {
    prime = (p * 2) + 3;
    for (uint64_t m = p + (prime * (prime / 2)); m < sieve_size; m += prime) {
      sieve[m] = false;
    }
  }

  uint64_t sieve_index = 2;

  prime = 7;
  for (; prime < root_max_value;) {
    if (sieve[sieve_index]) {
      primes[p_index] = prime;
      ++p_index;

      for (uint64_t m = sieve_index + (prime * (prime / 2)); m < sieve_size;
           m += prime) {
        sieve[m] = false;
      }
    }

    prime += wheel[wheel_i];
    sieve_index += wheel[wheel_i] / 2;
    wheel_i = (wheel_i + 1) % 8;
  }

  for (; sieve_index < sieve_size;) {
    if (sieve[sieve_index]) {
      primes[p_index] = (sieve_index * 2) + 3;
      ++p_index;
    }
    sieve_index += wheel[wheel_i] / 2;
    wheel_i = (wheel_i + 1) % 8;
  }

  primes.resize(p_index);

  return primes;
}

void print_primes_wheeled(uint64_t max_value) {
  uint64_t wheel[8] = {4, 2, 4, 2, 4, 6, 2, 6};
  uint64_t wheel_i = 0;
  // uint64_t sum[8] = {4, 6, 10, 12, 16, 22, 24, 30};
  // uint64_t turns = 0;

  std::cout << "2\n3\n5\n";

  uint64_t root_max_value = uint64_t(ceil(sqrt(max_value)));

  uint64_t sieve_size = (max_value / 2) - 1;
  std::vector<bool> sieve(sieve_size, true);

  uint64_t prime;
  for (uint64_t p = 0; p <= 1; ++p) {
    prime = (p * 2) + 3;
    for (uint64_t m = p + (prime * (prime / 2)); m < sieve_size; m += prime) {
      sieve[m] = false;
    }
  }

  uint64_t sieve_index = 2;

  prime = 7;
  for (; prime < root_max_value;) {
    if (sieve[sieve_index]) {
      std::cout << prime << '\n';

      for (uint64_t m = sieve_index + (prime * (prime / 2)); m < sieve_size;
           m += prime) {
        sieve[m] = false;
      }
    }

    prime += wheel[wheel_i];
    sieve_index += wheel[wheel_i] / 2;
    wheel_i = (wheel_i + 1) % 8;
  }

  for (; sieve_index < sieve_size;) {
    if (sieve[sieve_index]) {
      std::cout << (sieve_index * 2) + 3 << '\n';
    }
    sieve_index += wheel[wheel_i] / 2;
    wheel_i = (wheel_i + 1) % 8;
  }
}

void print_primes_wheeled2(uint64_t max_value) {
  uint64_t wheel[8] = {4, 2, 4, 2, 4, 6, 2, 6};
  uint64_t wheel_i = 0;

  std::cout << "2\n3\n5";

  uint64_t root_max_value = uint64_t(ceil(sqrt(max_value)));

  // All odd numbers
  uint64_t sieve_size = (max_value / 2) - 1;
  std::vector<bool> sieve(sieve_size, true);

  uint64_t prime;

  // Mark as not prime all multiples of 3 and 5
  for (uint64_t p = 0; p <= 1; ++p) {
    prime = (p * 2) + 3;
    for (uint64_t m = p + (prime * (prime / 2)); m < sieve_size; m += prime) {
      sieve[m] = false;
    }
  }

  uint64_t sieve_index = 2;
  uint64_t inner_wheel_i;
  prime = 7;
  while (prime < root_max_value) {
    if (sieve[sieve_index]) {
      std::cout << prime << '\n';

      inner_wheel_i = wheel_i;
      for (uint64_t m = sieve_index + (prime * (prime / 2)); m < sieve_size;) {
        sieve[m] = false;
        m += prime * (wheel[inner_wheel_i] / 2);
        inner_wheel_i = (inner_wheel_i + 1) % 8;
      }
    }

    prime += wheel[wheel_i];
    sieve_index += wheel_i / 2;
    wheel_i = (wheel_i + 1) % 8;
  }

  while (sieve_index < sieve_size) {
    if (sieve[sieve_index]) {
      std::cout << (sieve_index * 2) + 3 << '\n';
    }
    sieve_index += wheel[wheel_i] / 2;
    wheel_i = (wheel_i + 1) % 8;
  }
}

void print_primes_wheeled3(uint64_t max_value) {
  uint64_t rows = uint64_t(ceil(max_value / 30.0));
  // False if prime
  // An array of false variables
  auto nums = new bool[rows][30];
  // Handle 1
  nums[0][0] = true;
  uint w;      // Wheel index
  uint m;      // Next multiple of wheel prime
  uint64_t r;  // Row index
  // Handle 2
  std::cout << 2 << std::endl;
  for (r = 1; r < rows; ++r) {
    nums[r][1] = true;
    // std::cout << "\tmarking " << r * 30 + w + 1 << std::endl;
  }
  for (m = 3; m < 30; m += 2) {
    nums[0][m] = true;
    // std::cout << "marking " << m + 1 << std::endl;
    for (r = 1; r < rows; ++r) {
      nums[r][m] = true;
      // std::cout << "\tmarking " << r * 30 + m + 1 << std::endl;
    }
  }
  // Handle 3+
  for (w = 2; w < 30; w += 2) {
    if (!nums[0][w]) {
      std::cout << w + 1 << std::endl;
      // This number must be prime
      // Remove all prime spokes if one of the basis numbers
      if (w < 5) {
        for (r = 1; r < rows; ++r) {
          nums[r][w] = true;
          // std::cout << "\tmarking " << r * 30 + w + 1 << std::endl;
        }
      }
      // Remove all wheel composites and their spokes
      for (m = (w * w + 2 * w); m < 30; m += 2 * (w + 1)) {
        nums[0][m] = true;
        // std::cout << "marking " << m + 1 << std::endl;
        for (r = 1; r < rows; ++r) {
          nums[r][m] = true;
          // std::cout << "\tmarking " << r * 30 + m + 1 << std::endl;
        }
      }
    }
  }

  uint64_t prime_i = 6;
  uint64_t jumps[] = {4, 2, 4, 2, 4, 6, 2, 6};
  uint jump_i = 0;
  uint i;
  uint64_t max_sieve_value = uint64_t(ceil(sqrt(max_value)));
  while (prime_i + 1 < max_sieve_value) {
    // r = prime_i / 30;
    // w = prime_i % 30;
    //  If prime
    if (!nums[prime_i / 30][prime_i % 30]) {
      if (prime_i + 1 > 30) {
        std::cout << prime_i + 1 << std::endl;
      }
      m = (prime_i * prime_i + 2 * prime_i);
      // There are only 15 possible odd mod 30 values
      for (i = 0; m / 30 < rows || i < 15; ++i) {
        for (r = m / 30; r < rows; r += prime_i + 1) {
          nums[r][m % 30] = true;
          // std::cout << "\tmarking " << r * 30 + (m % 30) + 1 << std::endl;
        }

        m += 2 * (prime_i + 1);
      }
    }

    prime_i += jumps[jump_i];
    jump_i = (jump_i + 1) % 8;
  }

  while (prime_i < max_value) {
    if (!nums[prime_i / 30][prime_i % 30]) {
      std::cout << prime_i + 1 << "\n";
    }

    prime_i += jumps[jump_i];
    jump_i = (jump_i + 1) % 8;
  }
}

// Should include best guess of all prime numbers between [0, max_value) and
// sorted from smallest to largest. Returns the first non-prime number OR 0 if
// all are prime.
uint64_t all_prime(std::vector<uint64_t> primes) {
  uint64_t max_root = 0;
  for (uint64_t p = 0; p < primes.size(); ++p) {
    // std::cout << primes[p] << std::endl;
    max_root = uint64_t(ceil(sqrt(primes[p])));
    for (uint64_t t = 0; primes[t] < max_root; ++t) {
      if (primes[p] % primes[t] == 0) {
        return primes[p];
      }
    }
  }

  return 0;
}

int main(int argc, char* argv[]) {
  // for(uint64_t i = 10; i < 10000000000000000000; i *= 10) {
  //     std::cout << "10^" <<log(i) / log(10) << ": " << uint64_t(double(i) /
  //     (log(i) - 1.1)) << std::endl;
  // }

  // std::cout << "Running ..." << std::endl;
  uint64_t max_value;
  std::istringstream iss(argv[1]);
  iss >> max_value;
  /*
  uint64_t distance = 0;
  std::vector<uint64_t> distances;
  for(uint64_t n = 0; n < max_value - 3; n += 2) {
      std::cout << n << ": " << n + 3 << " | ";
      if(((n + 3) % 3 != 0) && ((n + 3) % 5 != 0) && ((n + 3) % 7 != 0) && ((n +
  3) % 11 == 0)) { std::cout << "first " << (distance / 11) * 2;
          distances.push_back((distance / 11) * 2);
          distance = 0;
      }
      ++distance;
      std::cout << std::endl;
  }

  for(uint64_t& d : distances) {
      std::cout << d << " ";
  }
  std::cout << std::endl;*/

  // auto start_gen = std::chrono::system_clock::now();
  // print_primes_bitmap(max_value);
  print_primes_count_vector(max_value);
  // print_primes_count_vector_2(max_value);
  // print_primes_vector(max_value);
  // print_primes_wheeled3(max_value);
  // print_primes_wheeled2(max_value);
  // std::vector<uint64_t> primes = gen_primes_wheeled(max_value);
  // std::cout << primes.size() << std::endl;
  /*for(uint64_t& prime : primes) {
      if(prime == 2) {
          continue;
      }
      if(prime % 2 == 0) {
          std::cout << prime << std::endl;
      }
      for(uint64_t i = 3; i < prime; i += 2) {
          if(prime % i == 0) {
              std::cout << prime << std::endl;
          }
      }
  }*/

  // std::cout << all_prime(primes) << std::endl;
  // auto end_gen = std::chrono::system_clock::now();
  // std::chrono::duration<double> elapsed = end_gen - start_gen;

  // std::cout << "serial: " << elapsed.count() << std::endl;
  // std::cout << "\tsize: " << primes.size() << std::endl;

  // std::cout << all_prime(primes) << std::endl;

  /*
  start_gen = std::chrono::system_clock::now();
  primes = gen_primes_parallel(max_value);
  end_gen = std::chrono::system_clock::now();
  elapsed = end_gen - start_gen;

  std::cout << "parallel: " << elapsed.count() << std::endl;
  std::cout << "\tsize: " << primes.size() << std::endl;
  */
  // for(uint64_t i = 0; i < primes.size(); ++i) {
  //     std::cout << primes[i] << std::endl;
  // }

  // std::cout << all_prime(primes) << std::endl;

  // uint64_t result = all_prime(primes);
  // std::cout << "total number of primes: " << primes.size() << std::endl;
  // std::cout << "all_prime() result: " << result << std::endl;
  // auto end_sieve = std::chrono::system_clock::now();
  // std::chrono::duration<double> elapsed_sieve = end_sieve - start_sieve;
  // std::cout << elapsed_sieve.count() << std::endl;

  // for(size_t& p : primes) {
  //     std::cout << p << std::endl;
  // }
  // std::cout << "done" << std::endl;
  return 0;
}
