/*
 * A372442  (Greatest binary index of n) minus (greatest prime index of n).
 * https://oeis.org/A372442
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = A070939(n) - A061395(n) Greatest binary index of n =
 *            floor(log2(n)) + 1 Greatest prime index of n = max index of a
 *            prime divisor of n a(n) = (floor(log2(n)) + 1) -
 *            max_prime_index_of(n) (n > 1)
 * Range:     i8 n <= 76, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes.h"

seq_t a(idx_t n)
{
    if (n < 2)   /* values below offset are irrelevant */
        return 0;

    /* --- Greatest binary index of n (floor(log2(n)) + 1) --- */
    idx_t log2 = 0;
    idx_t t = n;
    while (t > 1) {
        t >>= 1;
        log2++;
    }
    /* log2 is floor(log2(n)); add 1 for the 1‑based index */
    seq_t bin_idx = (seq_t)(log2 + 1);

    /* --- Prime table caching --- */
    static idx_t *primes = NULL;       /* array of primes up to primes_limit */
    static size_t   primes_cnt = 0;    /* number of primes in array       */
    static idx_t    primes_limit = 0;  /* maximum n for which primes stored */

    if (n > primes_limit) {
        /* Regenerate the sieve for the larger limit.
           Free old storage if any. */
        if (primes != NULL)
            free((void *)primes);

        build_primes(n, &primes, &primes_cnt);
        primes_limit = n;
    }

    /* --- Greatest prime index of n --- */
    idx_t temp = n;
    size_t maxPrimeIdx = 0;   /* 1‑based index, 0 if n==1 (not used) */

    /* Factor n using the prime list */
    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        /* Check p * p > temp without overflow */
        if (temp / p < p)
            break;
        if (temp % p == 0) {
            if (i + 1 > maxPrimeIdx)
                maxPrimeIdx = i + 1;
            /* Divide out all powers of p */
            do {
                temp /= p;
            } while (temp % p == 0);
            if (temp == 1)
                break;
        }
    }

    /* Remaining factor, if any, is a prime > sqrt(original n) */
    if (temp > 1) {
        /* Binary search for temp in primes[] */
        size_t lo = 0, hi = primes_cnt - 1;
        while (lo <= hi) {
            size_t mid = (lo + hi) >> 1;
            if (primes[mid] == temp) {
                if (mid + 1 > maxPrimeIdx)
                    maxPrimeIdx = mid + 1;
                break;
            } else if (primes[mid] < temp) {
                lo = mid + 1;
            } else {
                if (mid == 0) break;
                hi = mid - 1;
            }
        }
    }

    /* --- Return the difference --- */
    return bin_idx - (seq_t)maxPrimeIdx;
}
