import math
import sys

def gen_primes(max_value:int):
    primes = [2]
    root_max_value = math.ceil(math.sqrt(max_value))

    sieve_size = math.floor(max_value / 2) - 1
    sieve = [True] * sieve_size

    for i in range(root_max_value):
        if sieve[i]:
            prime = (i * 2) + 3
            primes.append(prime)
            for m in range(i + (prime * (prime // 2)), sieve_size, prime):
                sieve[m] = False

    for i in range(root_max_value, sieve_size):
        if sieve[i]:
            primes.append((i * 2) + 3)
    
    return primes

print(len(gen_primes(int(sys.argv[1]))))