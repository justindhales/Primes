#include <inttypes.h>
#include <stdio.h>

#include "primes.h"

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <max-value>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  size_t max_value = strtoumax(argv[1], NULL, 10);

  print_primes_bitmap(max_value);
  return 0;
}