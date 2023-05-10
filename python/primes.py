#!/usr/bin/env python3

import math
import sys
import time


def gen_primes(max_value: int):
    print("My version")
    # primes = [2]
    count = 1
    root_max_value = math.ceil(math.sqrt(max_value))

    sieve_size = math.floor(max_value / 2) - 1
    sieve = [True] * sieve_size

    ops = 0

    for i in range(root_max_value):
        if sieve[i]:
            prime = (i * 2) + 3
            count += 1
            # primes.append(prime)
            for m in range(i + (prime * (prime // 2)), sieve_size, prime):
                sieve[m] = False
                ops += 1

    print(f"Total operations: {ops}")

    for i in range(root_max_value, sieve_size):
        if sieve[i]:
            count += 1
            # primes.append((i * 2) + 3)

    print(f"Found {count} primes to {max_value}")

    # return primes
    return ops


def sieve_of_Sundaram_simple(n):
    """The sieve of Sundaram is a simple deterministic algorithm for finding all the prime numbers up to a specified integer."""
    print("Sundaram's version simple")
    k = (n - 2) // 2
    integers_list = [True] * (k + 1)
    ops = 0
    for i in range(1, k + 1):
        j = i
        while i + j + 2 * i * j <= k:
            integers_list[i + j + 2 * i * j] = False
            j += 1
            ops += 1

    print(f"Total operations: {ops}")

    count = 0
    if n > 2:
        count += 1
    for i in range(1, k + 1):
        if integers_list[i]:
            count += 1

    print(f"Found {count} primes to {n}")
    return ops


def sieve_of_Sundaram(n):
    """The sieve of Sundaram is a simple deterministic algorithm for finding all the prime numbers up to a specified integer."""
    print("Sundaram's version")
    if n < 3:
        if n < 2:
            return 0
        else:
            return 1
    k = (n - 3) // 2 + 1

    integers_list = [True for i in range(k)]

    ops = 0

    for i in range((int(math.sqrt(n)) - 3) // 2 + 1):
        #        if integers_list[i]: # adding this condition turns it into a SoE!
        p = 2 * i + 3
        s = (p * p - 3) // 2  # compute cull start

        for j in range(s, k, p):
            integers_list[j] = False
            ops += 1

    print(f"Total operations: {ops}")

    count = 1
    for i in range(k):
        if integers_list[i]:
            count += 1

    print(f"Found {count} primes to {n}")
    return ops


# print(len(gen_primes(int(sys.argv[1]))))
max_value = int(sys.argv[1])
start = time.time()
my_ops = gen_primes(max_value)
print(f"Time: {time.time() - start}")

start = time.time()
sandaram_ops = sieve_of_Sundaram(max_value)
print(f"Time: {time.time() - start}")

start = time.time()
simple_ops = sieve_of_Sundaram_simple(max_value)
print(f"Time: {time.time() - start}")

print(f"My version operation speedup: {sandaram_ops / my_ops}")
