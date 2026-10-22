/*
 * A356171  Odd numbers that are not divisible by two consecutive prime
 *          numbers.
 * https://oeis.org/A356171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th odd integer that has no two consecutive prime
 *            factors (odd numbers not divisible by two consecutive primes)
 * Range:     u8 n <= 112, u16 n <= 9445, u32 n <= 9445, u64 n <= 9445,
 *            u128 n <= 9445, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* Prime table, created once on the first call. */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;

/* Test whether a number has at least one pair of consecutive prime factors. */
static bool has_consecutive_prime_factors(uint64_t num)
{
    if (num <= 1) return false;            /* 1 has no prime factors */

    uint64_t tmp = num;
    uint64_t factors[32];
    size_t fcount = 0;

    /* Factor out small primes from the list. */
    for (size_t i = 0; i < prime_count && prime_list[i] <= tmp / prime_list[i]; ++i) {
        uint64_t p = prime_list[i];
        if (tmp % p == 0) {
            factors[fcount++] = p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) {                         /* remaining prime factor > sqrt(original) */
        factors[fcount++] = tmp;
    }

    /* For each distinct prime factor, check if the next prime also divides the number. */
    for (size_t i = 0; i < fcount; ++i) {
        uint64_t p = factors[i];

        /* Locate p in the prime list by binary search. */
        size_t lo = 0, hi = prime_count;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (prime_list[mid] <= p) lo = mid + 1; else hi = mid;
        }
        if (lo == 0) continue;             /* p not found – it is larger than our table */
        size_t idx = lo - 1;
        if (idx + 1 >= prime_count) continue; /* no next prime in our table */

        uint64_t next_p = prime_list[idx + 1];
        if (num % next_p == 0) return true;   /* consecutive pair detected */
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Build a generous prime table once. */
    if (!prime_list) {
        /* 1 000 000 is more than enough for values up to tens of thousands. */
        idx_t limit = 1000000;
        build_primes_upto(limit, &prime_list, &prime_count);
    }

    idx_t count = 0;
    uint64_t candidate = 1;               /* consider only odd integers */

    while (true) {
        if (!has_consecutive_prime_factors(candidate)) {
            ++count;
            if (count == n) return (seq_t)candidate;
        }
        candidate += 2;                 /* next odd number */
    }
}
