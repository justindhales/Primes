fn overestimate_pi_func(max_value: f64) -> usize {
    return (max_value / (max_value.ln() - 1.1)) as usize;
}

fn gen_primes(max_value: usize) -> Vec<usize> {
    let mv = max_value as f64;
    let primes_array_size = overestimate_pi_func(mv);
    let mut primes = vec![0; primes_array_size];
    primes[0] = 2;
    let mut p_index = 1usize;
    let root_max_value = mv.sqrt().ceil() as usize;

    let sieve_size = (max_value / 2) - 1;
    let mut sieve = vec![true; sieve_size];

    let mut prime;
    for i in 0..root_max_value {
        if sieve[i] {
            prime = (i * 2) + 3;
            primes[p_index] = prime;
            p_index += 1;
            for m in ((i + (prime * (prime / 2)))..sieve_size).step_by(prime) {
                sieve[m] = false;
            }
        }
    }

    for i in root_max_value..sieve_size {
        if sieve[i] {
            primes[p_index] = (i * 2) + 3;
            p_index += 1;
        }
    }

    primes.resize(p_index, 0);

    primes
}

fn main() {
    let args: Vec<String> = std::env::args().collect();
    assert_eq!(args.len(), 2);
    let primes = gen_primes(args[1].parse().unwrap());
    println!("{}", primes.len());
}