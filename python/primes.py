#!/usr/bin/env python3

import math
import sys
import time


def prime_count(max_value: int):
    if max_value <= 1:
        return 0
    if max_value <= 2:
        return 1

    # For 2
    count = 1

    max_value_root_index = int((math.sqrt(max_value) - 3) / 2)

    sieve_size = (max_value - 1) // 2
    sieve = [True] * sieve_size

    ops = 0

    for i in range(max_value_root_index + 1):
        if sieve[i]:
            count += 1
            prime = (i * 2) + 3
            for m in range((prime * prime // 2) - 1, sieve_size, prime):
                sieve[m] = False
                ops += 1
    print(f"Total operations: {ops}")

    for i in range(max_value_root_index + 1, sieve_size):
        if sieve[i]:
            count += 1
            # primes.append((i * 2) + 3)

    print(f"Found {count} primes to {max_value}")

    return count


def gen_primes(max_value: int):
    print("My version")
    if max_value <= 1:
        return 0
    # primes = [2]
    count = 1
    max_value_root_index = (int(math.ceil(math.sqrt(max_value))) - 3) // 2

    sieve_size = ((max_value - 3) // 2) + 1
    sieve = [True] * sieve_size

    # skip_ratio = 1 / 2
    # print(skip_ratio)

    ops = 0

    for i in range(max_value_root_index):
        if sieve[i]:
            prime = (i * 2) + 3

            # skip_ratio += (1 - skip_ratio) / prime
            # print(skip_ratio)

            count += 1
            # primes.append(prime)
            for m in range(i + (prime * (prime // 2)), sieve_size, prime):
                sieve[m] = False
                ops += 1
        else:
            ops += 1

    print(f"Total operations: {ops} (ratio: {ops/max_value})")

    for i in range(max_value_root_index, sieve_size):
        if sieve[i]:
            count += 1
            # primes.append((i * 2) + 3)

    print(f"Found {count} primes to {max_value}")

    # return primes
    return ops


def gen_primes_2(max_value: int):
    print("My version 2")
    # primes = [2, 3]
    count = 2

    #

    sieve_size = (max_value - 1) // 3
    if (max_value - 4) % 6 == 0:
        sieve_size -= 1

    sieve = [True] * sieve_size

    max_value_root_index = 0


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

    print(f"Total operations: {ops} (ratio: {ops/n})")

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
    print(f"Sieve size: {k}")

    integers_list = [True for i in range(k)]

    ops = 0

    max_value_root_index = (int(math.sqrt(n)) - 3) // 2 + 1
    print(f"Max value root index: {max_value_root_index}")

    for i in range(max_value_root_index):
        #        if integers_list[i]: # adding this condition turns it into a SoE!
        p = 2 * i + 3
        s = (p * p - 3) // 2  # compute cull start

        for j in range(s, k, p):
            integers_list[j] = False
            ops += 1

    print(f"Total operations: {ops} (ratio: {ops/n})")

    count = 1
    for i in range(k):
        if integers_list[i]:
            count += 1

    print(f"Found {count} primes to {n}")
    return ops


if __name__ == "__main__":
    max_value = int(sys.argv[1])
    start = time.time()
    prime_count(max_value)
    print(f"Time: {time.time() - start}")

    # start = time.time()
    # sandaram_ops = sieve_of_Sundaram(max_value)
    # print(f"Time: {time.time() - start}")

    # start = time.time()
    # simple_ops = sieve_of_Sundaram_simple(max_value)
    # print(f"Time: {time.time() - start}")

    # print(f"My version operation speedup: {sandaram_ops / my_ops}")
