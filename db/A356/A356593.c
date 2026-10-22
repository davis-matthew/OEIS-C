/*
 * A356593  Smallest k such that primorial(k) > n^2.
 * https://oeis.org/A356593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that primorial(k) > n^2
 * Range:     u8 n <= 14, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* All computations are performed in seq_t to preserve exactness. */
    seq_t target = (seq_t)n * (seq_t)n;

    /* Static cache for primes and their primorials. */
    static seq_t *primorials = NULL;
    static size_t sz = 0;
    static size_t cap = 0;
    static uint64_t lastPrime = 0;

    /* Initialise cache on first call. */
    if (sz == 0) {
        cap = 8;                         /* initial capacity */
        primorials = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!primorials)
            return (seq_t)0;              /* allocation error */
        primorials[0] = (seq_t)2;        /* 2# = 2 */
        sz = 1;
        lastPrime = 2ULL;
    }

    /* Extend the cache until the last primorial exceeds the target. */
    while (primorials[sz - 1] <= target) {
        /* Find next prime after lastPrime. */
        uint64_t p = lastPrime + 1;
        if (p <= 2ULL)
            p = 2ULL;
        else {
            if (p % 2ULL == 0ULL)
                ++p;
            while (!is_prime_small(p))
                p += 2ULL;
        }

        /* Grow storage if necessary. */
        if (sz == cap) {
            cap <<= 1; /* double capacity */
            seq_t *tmp = (seq_t *)realloc(primorials, sizeof(seq_t) * cap);
            if (!tmp)
                return (seq_t)0;              /* allocation error */
            primorials = tmp;
        }

        /* Compute new primorial. */
        seq_t newProd = primorials[sz - 1] * (seq_t)p;
        primorials[sz] = newProd;
        ++sz;
        lastPrime = p;
    }

    /* Search for the first primorial that exceeds the target. */
    size_t i;
    for (i = 0; i < sz; ++i) {
        if (primorials[i] > target) {
            return (seq_t)(i + 1); /* k is 1‑based */
        }
    }

    /* Should never happen. */
    return (seq_t)0;
}
