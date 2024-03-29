#include "primes.h"

void print_primes_bitmap(size_t max_value) {
  size_t max_value_root_index = ((size_t)(ceil(sqrt(max_value))) - 3) / 2;

  // 0, 1, 2, 3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
  // 20, 21, 22, 23, ...
  // 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41,
  // 43, 45, 47, 49, ...
  size_t sieve_size = ((max_value - 3) / 2) + 1;
  bitmap_t* sieve = bitmap_new(sieve_size);
  // Set all bits to 1
  bitmap_init(sieve, 0xff);
  // FILE* tmp = tmpfile();
  fprintf(stdout, "2\n");

  size_t prime;
  size_t i;  // index
  size_t m;  // multiple of index

  for (i = 0; i <= max_value_root_index; ++i) {
    if (bitmap_get(sieve, i)) {
      prime = (i * 2) + 3;
      fprintf(stdout, "%ld\n", prime);

      for (m = ((prime * prime) / 2) - 1; m <= sieve_size; m += prime) {
        bitmap_set(sieve, m, 0);
      }
    }
  }

  for (i = max_value_root_index + 1; i < sieve_size; ++i) {
    if (bitmap_get(sieve, i)) {
      fprintf(stdout, "%ld\n", (i * 2) + 3);
    }
  }

  // printf("%s", tmp);
}

void print_primes_bitmap2(size_t max_value) {
  size_t max_value_root_index = ((size_t)(ceil(sqrt(max_value))) - 3) / 2;

  // 0, 1, 2, 3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
  // 20, 21, 22, 23, ...
  // 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41,
  // 43, 45, 47, 49, ...
  size_t sieve_size = ((max_value - 3) / 2) + 1;
  uint8_t* sieve = (uint8_t*)malloc((sieve_size / 8) + 1);
  memset(sieve, 0, (sieve_size / 8) + 1);

  fprintf(stdout, "2\n");
  size_t prime;
  size_t i;  // index
  size_t m;  // multiple of index

  for (i = 0; i <= max_value_root_index; ++i) {
    if (!(sieve[i / 8] & 1 << (i % 8))) {
      prime = (i * 2) + 3;
      fprintf(stdout, "%ld\n", prime);

      for (m = ((prime * prime) / 2) - 1; m <= sieve_size; m += prime) {
        sieve[m / 8] |= 1 << (m % 8);
      }
    }
  }

  for (i = max_value_root_index + 1; i < sieve_size; ++i) {
    if (!(sieve[i / 8] & 1 << (i % 8))) {
      fprintf(stdout, "%ld\n", (i * 2) + 3);
    }
  }
}