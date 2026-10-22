/*
 * A309323  Expansion of Sum_{k>=1} phi(k) * x^k/(1 - x^k)^4, where phi =
 *          Euler totient function (A000010).
 * https://oeis.org/A309323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(n/d) * d * (d + 1) * (d + 2)/6
 * Range:     u8 n <= 9, u16 n <= 69, u32 n <= 1887, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A309323.
 * The result is cached so repeated calls for the same n are fast,
 * and calls can be made in any order. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/phi.h"   /* provides seq_t phi(idx_t m) */

/* Static cache for already computed terms.  Cache[n] == 0 indicates
 * that the value for index n has not yet been computed (the sequence
 * is strictly positive for n >= 1).  Index 0 is unused. */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Evaluates a single term using the divisor‐sum formula. */
static seq_t compute_term(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* first divisor d */
            idx_t d1 = d;
            idx_t d2 = n / d;
            seq_t phi1 = phi(n / d1);            /* phi(d2) */
            seq_t d1_t = (seq_t)d1;
            seq_t T1 = d1_t * (d1_t + 1) * (d1_t + 2) / 6;
            sum += phi1 * T1;

            if (d1 != d2) {
                /* second divisor d2 */
                seq_t phi2 = phi(n / d2);            /* phi(d1) */
                seq_t d2_t = (seq_t)d2;
                seq_t T2 = d2_t * (d2_t + 1) * (d2_t + 2) / 6;
                sum += phi2 * T2;
            }
        }
    }
    return sum;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;   /* offset is 1; term for n=0 is undefined */
    }

    /* Grow cache if necessary. */
    if (n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed; return 0 as a sentinel error value. */
            return 0;
        }
        /* Initialise new slots to 0 (meaning "not computed"). */
        for (size_t i = cache_size; i < new_size; ++i) {
            new_cache[i] = 0;
        }
        cache = new_cache;
        cache_size = new_size;
    }

    /* Return cached value if available. */
    if (cache[n] != 0) {
        return cache[n];
    }

    /* Compute, cache, and return. */
    seq_t val = compute_term(n);
    cache[n] = val;
    return val;
}
