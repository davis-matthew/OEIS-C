/*
 * A351788  a(1) = 1; a(n) = a(n-1) + Sum_{d|n, 1 < d < n} a(n/d) * a(d).
 * https://oeis.org/A351788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + Sum_{d|n, 1<d<n} a(n/d) * a(d), a(1)=1
 * Range:     u8 n <= 25, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function memoizes results in a static array so that calls
   with arbitrary indices return the correct term quickly. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 1‑based cache */
    static seq_t *cache = NULL;
    static idx_t limit   = 0;      /* allocated size (maximum index we can hold) */
    static idx_t highest = 0;      /* highest index that has been computed */

    /* Undefined for n == 0; return 0 for safety */
    if (n == 0)
        return (seq_t)0;

    /* Grow the cache if necessary */
    if (limit < n) {
        idx_t new_limit = n > 1 ? n * 2 : 1; /* safety factor */
        cache = (seq_t *)realloc(cache, (new_limit + 1) * sizeof(seq_t));
        if (!cache)
            exit(EXIT_FAILURE);
        limit = new_limit;
    }

    /* Initialise base case a(1) */
    if (highest == 0) {
        cache[1] = (seq_t)1;
        highest = 1;
    }

    /* Compute all missing terms up to n */
    for (idx_t i = highest + 1; i <= n; ++i) {
        seq_t sum = 0;
        if (i > 1) {
            /* Enumerate divisors d from 2 up to sqrt(i) */
            for (idx_t d = 2; d * d <= i; ++d) {
                if (i % d == 0) {
                    idx_t e = i / d;
                    if (e == d) {
                        /* Perfect square: d == e */
                        sum += cache[d] * cache[e];
                    } else {
                        /* Two distinct divisors: add both contributions */
                        sum += (cache[d] * cache[e]) * 2;
                    }
                }
            }
        }
        cache[i] = cache[i - 1] + sum;
    }

    highest = n;
    return cache[n];
}
