/*
 * A327531  a(n) = 1 if the prime indices of n are relatively prime, otherwise
 *          a(n) = n.
 * https://oeis.org/A327531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the prime indices of n are relatively prime; *
 *            otherwise a(n) = n
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   build_primes_upto, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Utility functions supplied by the build */
#include "../utils/build_primes_upto.h"
#include "../utils/gcd.h"

/* Find the 1‑based index of a prime p in the sorted prime array.
   It is guaranteed that p is prime and present in primes. */
static inline idx_t
prime_index(const idx_t *primes, size_t cnt, idx_t p)
{
    idx_t lo = 0, hi = (idx_t)cnt - 1;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (primes[mid] == p) return mid + 1;           /* 1‑based */
        if (primes[mid] < p) lo = mid + 1;
        else hi = mid - 1;
    }
    return (idx_t)0;   /* should never happen */
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;          /* a(1) = 1 */

    /* ----------  cache of primes up to current maximum n ---------- */
    static idx_t cache_limit = 0;
    static idx_t *primes = NULL;
    static size_t prime_count = 0;

    if (n > cache_limit) {
        free(primes);                     /* free old cache if any */
        build_primes_upto(n, &primes, &prime_count);
        cache_limit = n;
    }

    /* ----------  factor n and collect distinct prime indices  ---------- */
    idx_t tmp = n;
    seq_t g = 0;                          /* gcd of prime indices */

    /* trial division by primes <= sqrt(tmp) */
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = primes[i];
        if (p * p > tmp) break;
        if (tmp % p == 0) {
            /* first occurrence of prime p */
            idx_t idx_val = prime_index(primes, prime_count, p);
            g = (g == 0) ? idx_val : gcd(g, idx_val);

            /* remove all powers of p from tmp */
            while (tmp % p == 0) tmp /= p;
        }
    }

    /* if a prime factor remains > sqrt(original n) */
    if (tmp > 1) {
        idx_t idx_val = prime_index(primes, prime_count, tmp);
        g = (g == 0) ? idx_val : gcd(g, idx_val);
    }

    return (g == 1) ? (seq_t)1 : (seq_t)n;
}
