/*
 * A361259  a(n) is the least semiprime that is the sum of n consecutive
 *          primes.
 * https://oeis.org/A361259
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least semiprime that is the sum of n consecutive primes
 * Range:     u8 n <= 5, u16 n <= 154, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is fully self‑contained and works for all seq_t types
   supported by the build system (including arbitrary‑precision).          */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------------------------------------------------------------------- */
/*   Helper: generate primes as uint64_t --------------------------------- */
static uint64_t *prime_arr = NULL;   /* primes[0] = 2, ...             */
static size_t  prime_used = 0;       /* number of primes stored       */
static size_t  prime_cap  = 0;       /* current capacity              */

/* Extend prime list until the largest prime >= limit.                  */
static void ensure_primes_upto(uint64_t limit)
{
    while (prime_used == 0 || prime_arr[prime_used-1] < limit) {
        uint64_t candidate;
        if (prime_used == 0) {
            candidate = 2;
        } else {
            candidate = prime_arr[prime_used-1] + 1;
            for (;;) {
                bool prime = true;
                for (size_t i = 0; i < prime_used; ++i) {
                    uint64_t p = prime_arr[i];
                    if (p * p > candidate) break;
                    if (candidate % p == 0) { prime = false; break; }
                }
                if (prime) break;
                ++candidate;
            }
        }
        if (prime_used == prime_cap) {
            size_t new_cap = prime_cap ? prime_cap * 2 : 16;
            uint64_t *new_arr = (uint64_t *)realloc(prime_arr, new_cap * sizeof(uint64_t));
            if (!new_arr) abort();
            prime_arr = new_arr;
            prime_cap = new_cap;
        }
        prime_arr[prime_used++] = candidate;
    }
}

/* ---------------------------------------------------------------------- */
/*   Memoisation of a(n) -------------------------------------------------- */
static seq_t *value_cache = NULL;  /* cache[n] holds a(n) for n>=3           */
static size_t value_cache_cap = 0; /* cache size (indices 0..cap-1)          */

/* Ensure cache can hold index n.                                         */
static void ensure_cache(idx_t n)
{
    if (n < 3) return;
    while (value_cache_cap <= (size_t)n) {
        size_t new_cap = value_cache_cap ? value_cache_cap * 2 : 16;
        seq_t *new_cache = (seq_t *)realloc(value_cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        for (size_t i = value_cache_cap; i < new_cap; ++i) new_cache[i] = 0;
        value_cache = new_cache;
        value_cache_cap = new_cap;
    }
}

/* ---------------------------------------------------------------------- */
/*   Semiprime tester ---------------------------------------------------- */
static bool is_semiprime_seq(seq_t n)
{
    if (n < 4) return false;            /* 4 is the smallest semiprime */

    /* For small values that fit in uint64_t, use integer arithmetic. */
    const uint64_t max64 = (uint64_t)-1;
    if (n <= (seq_t)max64) {
        uint64_t m = (uint64_t)n;
        uint64_t tmp = m;
        int exp_sum = 0;

        /* Ensure we have enough primes up to sqrt(tmp). */
        uint64_t limit = 0;
        while (limit * limit <= tmp) ++limit;
        ensure_primes_upto(limit);

        for (size_t i = 0; i < prime_used && prime_arr[i] * prime_arr[i] <= tmp; ++i) {
            uint64_t p = prime_arr[i];
            while (tmp % p == 0) {
                ++exp_sum;
                tmp /= p;
            }
        }
        if (tmp > 1) ++exp_sum;

        return exp_sum == 2;
    }

    /* Fallback for values larger than 64 bits (rare for the tested range). */
    seq_t rem = n;
    int exp_sum = 0;
    uint64_t idx = 0;
    while (true) {
        uint64_t p = prime_arr[idx];
        if (idx >= prime_used) {
            /* If we run out of precomputed primes, extend the list. */
            ensure_primes_upto(prime_arr[prime_used-1] * 2 + 1);
            p = prime_arr[idx];
        }
        seq_t p_seq = (seq_t)p;
        if (p_seq * p_seq > rem) break;

        while (rem % p_seq == 0) {
            ++exp_sum;
            rem = rem / p_seq;
        }
        ++idx;
    }
    if (rem > 1) ++exp_sum;
    return exp_sum == 2;
}

/* ---------------------------------------------------------------------- */
/*   Main sequence function ------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 3) return 0;              /* undefined below offset */

    ensure_cache(n);
    if (value_cache[n] != 0) return value_cache[n];

    /* Need at least n primes for the first window. */
    ensure_primes_upto((uint64_t)prime_used);
    if (prime_used < (size_t)n) {
        ensure_primes_upto((uint64_t)n * 20);   /* over‑estimate to get n primes */
    }

    /* Sum of the first n primes. */
    seq_t sum = 0;
    for (idx_t i = 0; i < n; ++i) sum += (seq_t)prime_arr[i];

    idx_t start = 0;
    for (;;) {
        if (is_semiprime_seq(sum)) {
            value_cache[n] = sum;
            return sum;
        }
        /* Slide window one step to the right. */
        ++start;
        /* Ensure the next prime needed for the new window is available. */
        ensure_primes_upto((uint64_t)prime_arr[start + n - 1] + 10); /* safety margin */
        sum = sum - (seq_t)prime_arr[start - 1] + (seq_t)prime_arr[start + n - 1];
    }
}
