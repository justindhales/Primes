function overestimate_pi_func(max_value)
    return floor(Int64, max_value / (log(max_value) - 1.1))
end

function gen_primes(max_value)
    primes_array_size = overestimate_pi_func(max_value)

    primes = zeros(Int64, primes_array_size)
    primes[0] = 2
    p_index = 1
    root_max_value = ceil(Int64, sqrt(max_value))

    sieve_size = (max_value // 2) - 1
    sieve = trues(sieve_size)

    for i in range(root_max_value)
        if sieve[i]
            prime = (i * 2) + 3
            primes[p_index] = prime
            p_index += 1
            m = i + (prime * (prime // 2))
            while m < sieve_size
                sieve[m] = false
                m += prime
            end
        end
    end
    
    for i = root_max_value:sieve_size-1
        if sieve[i]
            primes[p_index] = (i * 2) + 3
            p_index += 1
        end
    end

    resize!(primes, p_index)

    return primes
end

println(gen_primes(10))