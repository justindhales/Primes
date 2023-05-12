use core::time;
use std::{
    sync::{mpsc, Arc, Mutex},
    thread::{self}, time::Instant,
};

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
    let sieve_size = (max_value - 1) / 2;
    let max_value_root_index = (((max_value as f64).sqrt() - 3.0) / 2.0) as usize;

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

fn primes_count_two_threads(max_value: usize) -> usize {
    if max_value <= 1 {
        return 0;
    } else if max_value <= 2 {
        return 1;
    }

    let mut count = 1usize;

    let sieve_size = (max_value - 1) / 2;

    let root_sieve_size = (((max_value as f64).sqrt() - 3.0) / 2.0) as usize + 1;
    let upper_sieve_size = sieve_size - root_sieve_size;

    let mut root_sieve = bitvec![1; root_sieve_size];
    let upper_sieve = Arc::new(Mutex::new(bitvec![1; upper_sieve_size]));

    let (tx, rx) = mpsc::channel();

    let upper_sieve_clone = Arc::clone(&upper_sieve);
    let handle = thread::spawn(move || {
        let mut upper_sieve = upper_sieve_clone.lock().unwrap();
        loop {
            let (start_i, prime) = rx.recv().unwrap();
            println!("Thread starting");
            // println!("thread - prime: {} start: {}", prime, (start_i * 2) + 3);
            if prime == 0 {
                break;
            }

            for m in (start_i..upper_sieve.len()).step_by(prime) {
                upper_sieve.set(m, false);
            }
        }
    });

    for i in 0..root_sieve.len() {
        if root_sieve[i] {
            count += 1;
            let prime = (i * 2) + 3;
            let mut m = (prime * prime / 2) - 1;
            // println!(
            //     "main - prime: {} square: {} root_sieve.len(): {}",
            //     prime,
            //     (m * 2) + 3,
            //     root_sieve.len()
            // );
            println!("Main starting");
            while m < root_sieve.len() {
                root_sieve.set(m, false);
                m += prime;
            }
            // Signal the thread to compute the other part
            match tx.send((m - root_sieve.len(), prime)) {
                Err(e) => panic!("Could not send pair: {:?}", e),
                _ => (),
            }

            // thread::sleep(time::Duration::from_secs(1));
        }
    }

    // Signal the thread to close
    match tx.send((0, 0)) {
        Err(e) => panic!("Could not signal to close: {:?}", e),
        _ => (),
    }

    match handle.join() {
        Err(e) => panic!("Could not join worker thread: {:?}", e),
        _ => (),
    }

    {
        let upper_sieve = upper_sieve.lock().unwrap();
        for i in 0..upper_sieve.len() {
            if upper_sieve[i] {
                // println!("{}", (i * 2) + 3);
                count += 1;
            }
        }
    }

    count
}

// fn primes_count_threaded(max_value: usize, num_threads: usize) -> usize {
//     let sieve_size = ((max_value - 3) / 2) + 1;
//     let max_value_root_index = (((max_value as f64).sqrt().ceil() as usize) - 3) / 2;

//     let mut sieve = bitvec![1; sieve_size];

//     let mut count = 1usize;

//     let mut shared_data = Arc::new((bitvec![1; sieve_size],));

//     let mut last_square = Arc::new((Mutex::new(4usize), Condvar::new()));

//     let mut handles = Vec::new();

//     for _ in 0..num_threads {
//         let last_square = last_square.clone();
//         let handle = thread::spawn(move || {
//             *last_square.0.lock().unwrap() += 1;
//         });
//         handles.push(handle);
//     }

//     for handle in handles {
//         handle.join().unwrap();
//     }

//     println!("{}", *last_square.0.lock().unwrap());

//     for i in 0..=max_value_root_index {
//         if sieve[i] {
//             count += 1;
//             let prime = (i * 2) + 3;
//             for m in (((prime * prime / 2) - 1)..sieve_size).step_by(prime) {
//                 sieve.set(m, false);
//             }
//         }
//     }

//     for i in (max_value_root_index + 1)..sieve_size {
//         if sieve[i] {
//             count += 1;
//         }
//     }

//     count
// }

fn main() {
    let args: Vec<String> = std::env::args().collect();
    assert!(args.len() >= 2);
    let max_value = args[1].parse::<usize>().unwrap();
    let now = Instant::now();
    println!(
        "Threaded: Found {} primes to {}",
        primes_count_two_threads(max_value),
        max_value
    );
    println!("Took {} seconds", now.elapsed().as_secs_f64());
    let now = Instant::now();
    println!(
        "Serial: Found {} primes to {}",
        primes_count(max_value),
        max_value
    );
    println!("Took {} seconds", now.elapsed().as_secs_f64());
}
