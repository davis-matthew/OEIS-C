/*
 * A345284  a(n) = Sum_{p|n, p prime} (p #).
 * https://oeis.org/A345284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, p prime} p# (primorial of p)
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 28, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* build_primes_upto   : build array of all primes <= limit.
   It allocates the array (via malloc) and returns it in primes_out,
   and its length in cnt_out. */
#include "../utils/build_primes_upto.h"

/* static cache of primes up to max_cached */
static idx_t          max_cached = 0;     /* largest n for which primes are cached */
static size_t         prime_cnt  = 0;     /* number of primes cached      */
static uint64_t      *prime_arr  = NULL;  /* array of primes              */

/* Binary search for the index of a prime (must exist in prime_arr). */
static size_t find_prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_arr[mid] == p) return mid;
        if (prime_arr[mid] < p)   lo = mid + 1;
        else                      hi = mid;
    }
    return (size_t)-1;   /* should not happen for prime factors of n */
}

/* Ensure that prime_arr contains all primes <= n. */
static void ensure_cache(idx_t n)
{
    if (n <= max_cached) return;           /* nothing to do */

    /* free the old cache if any */
    free(prime_arr);
    prime_arr = NULL;
    prime_cnt = 0;

    /* build new list of primes up to n */
    build_primes_upto(n, &prime_arr, &prime_cnt);
    max_cached = n;
}

/* Compute the primorial of a prime p (i.e., product of all primes ≤ p).   */
static seq_t primorial_of(uint64_t p)
{
    /* p is guaranteed to be a prime ≤ max_cached */
    size_t idx = find_prime_index(p);
    if (idx == (size_t)-1) return 0;   /* safety guard, should not happen */

    seq_t prod = 1;                     /* start with 1 as seq_t */
    for (size_t i = 0; i <= idx; ++i) {
        prod *= (seq_t)prime_arr[i];    /* multiply by each prime */
    }
    return prod;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;   /* a(1) = 0, a(0) unused */

    /* ensure we have all primes up to n cached */
    ensure_cache(n);

    idx_t x = n;
    seq_t sum = 0;

    /* factor n using cached primes */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_arr[i];
        if (p > x / p) break;   /* stop when p > sqrt(x) */

        if (x % p == 0) {
            sum += primorial_of(p);
            while (x % p == 0) x /= p;   /* strip all copies of p */
        }
    }

    /* if there's a remaining prime factor > sqrt(original n) */
    if (x > 1) {
        sum += primorial_of(x);
    }

    return sum;
}
