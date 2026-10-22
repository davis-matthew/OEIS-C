/*
 * A308754  a(0) = 0, a(n) = a(n-1) + 1 if 2*n + 3 is prime, otherwise a(n) =
 *          a(n-1).
 * https://oeis.org/A308754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0 a(n) = a(n-1) + 1 if 2*n + 3 is prime, otherwise a(n) =
 *            a(n-1)
 * Range:     u8 n <= 811, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>      /* for size_t */
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;   /* array of a(i) for i = 0 .. last_computed */
    static idx_t  cacheSize = 0;  /* number of elements in cache (>=1 if initialized) */

    /* initialize cache on first call */
    if (cacheSize == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0; /* out of memory: return 0 (unlikely for typical usage) */
        cache[0] = 0;
        cacheSize = 1;         /* cache[0] valid */
    }

    /* extend cache if n is beyond what we've already computed */
    if (n >= cacheSize) {
        idx_t newSize = n + 1;                     /* need indices 0 .. n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, newSize * sizeof(seq_t));
        if (!tmp) return 0; /* out of memory: return 0 */
        cache = tmp;

        /* compute missing values */
        for (idx_t i = cacheSize; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            idx_t test = 2 * i + 3;
            if (is_prime(test)) {
                cache[i] = prev + 1;
            } else {
                cache[i] = prev;
            }
        }
        cacheSize = newSize;
    }

    return cache[n];
}
