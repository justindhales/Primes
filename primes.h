//
// Created by jdh on 6/17/20.
//

#ifndef PRIMES_PRIMES_H
#define PRIMES_PRIMES_H
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <thread>

namespace primes {
    // Finds at least all of the prime numbers below primesBelow
    // Because they are found in batches, there may be prime numbers above primesBelow
    void GenerateAndSavePrimes(size_t primesBelow) {
        std::vector<size_t> p;
        std::ifstream f("primes.txt");
        std::string line;
        size_t num;
        std::cout << "Loading previous primes..." << std::endl;
        while(!f.eof()) {
            f >> num;
            p.push_back(num);
        }
        f.close();
        std::cout << "Done loading previous primes." << std::endl;
        size_t totalBefore = p.size();

        size_t test = p.back();
        size_t add;
        if(p.size() % 2 == 0) {
            test += 4;
            add = 2;
        }
        else {
            test += 2;
            add = 4;
        }

        double squareRoot;
        while(test < primesBelow) {
            for(unsigned l = 0; l < 50000; ++l) { // Do batches of 50000 number checks
                squareRoot = sqrt(test);
                for(unsigned t = 2; ; ++t) {
                    if(test % p[t] == 0) {
                        // Not a new prime number; break out of loop
                        break;
                    }
                    else if(p[t] > squareRoot) {
                        // This is a new prime number
                        p.push_back(test);
                        break;
                    }
                }

                test += add;
                if(add == 4) {
                    add = 2;
                }
                else {
                    add = 4;
                }
            }

            std::cout << "\rFound " << (test * 100) / primesBelow << "%, latest: " << p.back();
        }

        std::cout << "\rFound 100%, latest: " << p.back() << std::endl;

        // Open file for appending
        std::ofstream o("primes.txt", std::ios::app);
        std::cout << "Writing numbers to file..." << std::endl;
        for(unsigned i = totalBefore; i < p.size(); ++i) {
            o << "\n" << p[i];
        }
        std::cout << "Done." << std::endl;
        o.close();
    }

    // Function that will find all prime numbers between a lower and upper bound and insert them into a vector passed
    // by reference
    void GeneratePrimesThread(const std::vector<size_t>& primes, std::vector<size_t>& newPrimes, size_t start, size_t end) {
        // Find the first number not divisible by 2 or 3
        size_t test = start + 1;
        while(test % 2 == 0 || test % 3 == 0) {
            ++test;
        }

        size_t add;
        // Figure out what part of the 2-3 cycle the number is on
        if(test - 1 % 3 == 0) {
            add = 4;
        }
        else {
            add = 2;
        }

        double squareRoot;
        while(test < end) {
            squareRoot = sqrt(test);
            for(unsigned t = 2; ; ++t) {
                if(test % primes[t] == 0) {
                    // Not a new prime number; break out of loop
                    break;
                }
                else if(primes[t] > squareRoot) {
                    // This is a new prime number
                    newPrimes.push_back(test);
                    break;
                }
            }

            test += add;
            if(add == 4) {
                add = 2;
            }
            else {
                add = 4;
            }
        }
    }

    void GenerateAndSavePrimesThreaded(size_t primesBelow) {
        std::vector<size_t> p;
        std::ifstream f("primes.txt");
        std::string line;
        size_t num;
        std::cout << "Loading previous primes..." << std::endl;
        while(!f.eof()) {
            f >> num;
            p.push_back(num);
        }
        f.close();
        std::cout << "Done loading previous primes." << std::endl;
        size_t totalBefore = p.size();

        std::vector<size_t> thread1;
        std::vector<size_t> thread2;
        std::vector<size_t> thread3;
        std::vector<size_t> thread4;
        size_t division;
        bool last = false;

        while(!last) {
            std::cout << "size: " << p.size() << std::endl;
            std::cout << "last: " << p.back() << std::endl;
            if(p.back() * p.back() < primesBelow) {
                division = (p.back() * p.back() - p.back() - 1) / 4;
            }
            else {
                division = (primesBelow - p.back()) / 4;
                last = true;
            }

            std::cout << "division: " << division << std::endl;
            std::thread first (GeneratePrimesThread, std::ref(p), std::ref(thread1), p.back(), p.back() + division - 1);
            std::thread second (GeneratePrimesThread, std::ref(p), std::ref(thread2), p.back() + division, p.back() + 2 * division - 1);
            std::thread third (GeneratePrimesThread, std::ref(p), std::ref(thread3), p.back() + 2 * division, p.back() + 3 * division - 1);
            std::thread fourth (GeneratePrimesThread, std::ref(p), std::ref(thread4), p.back() + 3 * division, p.back() + 4 * division);

            first.join();
            std::cout << "first ended" << std::endl;
            second.join();
            std::cout << "second ended" << std::endl;
            third.join();
            std::cout << "third ended" << std::endl;
            fourth.join();
            std::cout << "fourth ended" << std::endl;

            p.insert(p.end(), thread1.begin(), thread1.end());
            p.insert(p.end(), thread2.begin(), thread2.end());
            p.insert(p.end(), thread3.begin(), thread3.end());
            p.insert(p.end(), thread4.begin(), thread4.end());

            thread1.clear();
            thread2.clear();
            thread3.clear();
            thread4.clear();
        }

        std::cout << "size: " << p.size() << std::endl;
        std::cout << "last: " << p.back() << std::endl;

        // Open file for appending
        std::ofstream o("primes.txt", std::ios::app);
        std::cout << "Writing numbers to file..." << std::endl;
        for(unsigned i = totalBefore; i < p.size(); ++i) {
            o << "\n" << p[i];
        }
        std::cout << "Done." << std::endl;
        o.close();
    }
}

#endif //PRIMES_PRIMES_H
