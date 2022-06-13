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
  FILE* tmp = tmpfile();
  fprintf(tmp, "2\n");

  size_t prime;
  size_t i;  // index
  size_t m;  // multiple of index

  for (i = 0; i <= max_value_root_index; ++i) {
    if (bitmap_get(sieve, i)) {
      prime = (i * 2) + 3;
      fprintf(tmp, "%ld\n", prime);
    }
  }
}