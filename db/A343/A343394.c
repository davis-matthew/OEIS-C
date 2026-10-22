/*
 * A343394  Sum of indices of n's distinct prime factors below n.
 * https://oeis.org/A343394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n is prime; otherwise the sum of the 1‑based indices
 *            of the distinct prime factors of n (using the prime list sorted
 *            in ascending order)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

/* static storage for the prime table: primes[0] = 2, primes[1] = 3, … */
static uint64_t *s_primes   = NULL;   /* array of primes (ascending)      */
static size_t    s_prime_cnt = 0;     /* number of primes stored          */
static idx_t     s_max_n     = 0;     /* maximum n for which primes are known */

/* extend the prime table so that all primes <= n are known */
static void extend_primes(idx_t n)
{
    if (n <= s_max_n)      /* already enough */
        return;

    uint64_t *new_primes = NULL;
    size_t   new_cnt     = 0;

    build_primes_upto(n, &new_primes, &new_cnt);   /* allocates new array */

    free(s_primes);      /* free old array, if any */
    s_primes   = new_primes;
    s_prime_cnt = new_cnt;
    s_max_n     = n;
}

/* binary search in the prime table; returns index of p if found,
   otherwise returns s_prime_cnt (i.e., not found) */
static size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = s_prime_cnt;
    while (lo < hi) {
        size_t mid = lo + ((hi - lo) >> 1);
        uint64_t val = s_primes[mid];
        if (val == p)   return mid;
        if (val < p)    lo = mid + 1;
        else            hi = mid;
    }
    return s_prime_cnt;    /* not found */
}

/* main function required by the problem statement */
seq_t a(idx_t n)
{
    if (n < 2)          /* a(1) = 0 by definition */
        return (seq_t)0;

    extend_primes(n);

    /* test whether n itself is prime */
    size_t idx = prime_index(n);
    if (idx < s_prime_cnt && s_primes[idx] == n)
        return (seq_t)0;

    seq_t sum = 0;
    idx_t temp = n;

    /* trial division by primes <= sqrt(temp) */
    for (size_t i = 0; i < s_prime_cnt; ++i) {
        uint64_t p = s_primes[i];
        if ((idx_t)p * (idx_t)p > temp)
            break;
        if (temp % p == 0) {
            sum += (seq_t)(i + 1);   /* indices are 1-based */
            while (temp % p == 0)
                temp /= p;
        }
    }

    /* remaining factor > sqrt(n) must be prime */
    if (temp > 1) {
        size_t rem_idx = prime_index(temp);
        if (rem_idx < s_prime_cnt && s_primes[rem_idx] == temp)
            sum += (seq_t)(rem_idx + 1);
    }

    return sum;
}
