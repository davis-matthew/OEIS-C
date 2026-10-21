/*
 * A273227  Consider all ways of writing the n-th composite number as the
 *          product of two divisors d1*d2 = d3*d4 = ...; a(n) is the minimum
 *          of the sums {d1 + d2, d3 + d4, ...}.
 * https://oeis.org/A273227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 415, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: for the n-th composite number m, return the minimum of
   d + m/d over all divisor pairs d * (m/d) = m. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "../utils/is_prime.h"          /* bool is_prime(idx_t x) */

/* Integer square root (floor) of a 64‑bit unsigned integer. */
static inline uint64_t isqrt64(uint64_t n)
{
    uint64_t l = 0, r = n;
    while (l < r) {
        uint64_t mid = l + ((r - l + 1) >> 1);
        if (mid <= n / mid)
            l = mid;
        else
            r = mid - 1;
    }
    return l;
}

/* Largest divisor of m that does not exceed sqrt(m).  m is guaranteed
   to be composite (m >= 4). */
static inline uint64_t largest_divisor_le_sqrt(uint64_t m)
{
    uint64_t limit = isqrt64(m);
    for (uint64_t d = limit; d >= 2; --d) {
        if (m % d == 0)
            return d;
    }
    return 1; /* never reached for composite m */
}

seq_t a(idx_t n)
{
    if (n == 0)              /* undefined for n=0, return 0 */
        return (seq_t)0;

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of entries currently stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t last_checked = 3; /* integer to test next (composite counter) */
    static idx_t composites_done = 0; /* number of composites processed */

    /* grow cache sufficiently to hold n entries */
    if (cache_len < (size_t)n) {
        size_t new_cap = cache_cap ?: 8;
        while (new_cap < (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;            /* allocation failure, return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* generate composite numbers up to the required n */
    while (composites_done < (size_t)n) {
        ++last_checked;               /* next integer to examine */
        if (!is_prime(last_checked)) { /* composite */
            ++composites_done;
            uint64_t m = last_checked;        /* composite value */
            uint64_t d = largest_divisor_le_sqrt(m);
            seq_t sum = (seq_t)d + (seq_t)(m / d);
            cache[composites_done - 1] = sum;
        }
    }

    /* update cache length */
    cache_len = (size_t)n;

    return cache[n - 1];
}
