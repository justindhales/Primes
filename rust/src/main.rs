use core::num;
use std::{thread::{self, JoinHandle}, sync::{Arc, Mutex, Condvar}, ops::DerefMut};

use bitvec::prelude::*;

fn overestimate_pi_func(max_value: f64) -> usize {
    return (max_value / (max_value.ln() - 1.1)) as usize;
}

fn gen_primes(max_value: usize) -> Vec<usize> {
    let mv = max_value as f64;
    let primes_array_size = overestimate_pi_func(mv);
    let mut primes = vec![0; primes_array_size];
    primes[0] = 2;
    let mut p_index = 1usize;
    let max_value_root_index = ((mv.sqrt().ceil() as usize) - 3) / 2;

    let sieve_size = (max_value / 2) - 1;
    let mut sieve = vec![true; sieve_size];

    let mut prime;
    for i in 0..=max_value_root_index {
        if sieve[i] {
            prime = (i * 2) + 3;
            primes[p_index] = prime;
            p_index += 1;
            for m in (((prime * prime / 2) - 1)..sieve_size).step_by(prime) {
                sieve[m] = false;
            }
        }
    }

    for i in (max_value_root_index + 1)..sieve_size {
        if sieve[i] {
            primes[p_index] = (i * 2) + 3;
            p_index += 1;
        }
    }

    primes.resize(p_index, 0);

    primes
}

fn primes_count(max_value: usize) -> usize {
    let sieve_size = ((max_value - 3) / 2) + 1;
    let max_value_root_index = (((max_value as f64).sqrt().ceil() as usize) - 3) / 2;

    let mut sieve = bitvec![1; sieve_size];

    let mut count = 1usize;

    for i in 0..=max_value_root_index {
        if sieve[i] {
            count += 1;
            let prime = (i * 2) + 3;
            for m in (((prime * prime / 2) - 1)..sieve_size).step_by(prime) {
                sieve.set(m, false);
            }
        }
    }

    for i in (max_value_root_index + 1)..sieve_size {
        if sieve[i] {
            count += 1;
        }
    }

    count
}

fn primes_count_threaded(max_value: usize, num_threads: usize) -> usize {
    let sieve_size = ((max_value - 3) / 2) + 1;
    let max_value_root_index = (((max_value as f64).sqrt().ceil() as usize) - 3) / 2;

    let mut sieve = bitvec![1; sieve_size];

    let mut count = 1usize;

    let mut shared_data = Arc::new((bitvec![1; sieve_size], ));

    let mut last_square = Arc::new((Mutex::new(4usize), Condvar::new()));

    let mut handles = Vec::new();

    for _ in 0..num_threads {
        let last_square = last_square.clone();
        let handle = thread::spawn(move || {
            *last_square.0.lock().unwrap() += 1;
        });
        handles.push(handle);
    }

    for handle in handles {
        handle.join().unwrap();
    }

    println!("{}", *last_square.0.lock().unwrap());

    for i in 0..=max_value_root_index {
        if sieve[i] {
            count += 1;
            let prime = (i * 2) + 3;
            for m in (((prime * prime / 2) - 1)..sieve_size).step_by(prime) {
                sieve.set(m, false);
            }
        }
    }

    for i in (max_value_root_index + 1)..sieve_size {
        if sieve[i] {
            count += 1;
        }
    }

    count
}

fn main() {
    let args: Vec<String> = std::env::args().collect();
    assert_eq!(args.len(), 2);
    let max_value = args[1].parse::<usize>().unwrap();
    println!("Found {} primes to {}", primes_count(max_value), max_value);
    primes_count_threaded(10, 100);
    // let primes = gen_primes(args[1].parse().unwrap());
    // println!("{}", primes.len());
}
