/*
 * A335283  a(n) = 1 + Sum_{d|n, n/d odd, d < n} a(d).
 * https://oeis.org/A335283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{d|n, n/d odd, d < n} a(d) with a(1) = 1. If n is
 *            even then a(n) = a(n/2). The implementation uses an incremental
 *            dynamic programming approach with array caching
 * Range:     u8 n <= 944, u16 n <= 9844, u32 n <= 9844, u64 n <= 9844,
 *            u128 n <= 9844, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle possible zero argument (not part of OEIS offset) */
    if (n == 0)
        return 0;

    /* static cache of computed values */
    static seq_t *cache = NULL;      /* cache[i] holds a(i) for 1 <= i <= cap-1 */
    static size_t cap = 0;           /* current capacity of cache (in elements) */
    static idx_t last = 0;           /* last index for which a(i) is computed */

    /* grow the cache if necessary */
    if (n + 1 > cap) {
        size_t new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* compute values up to n incrementally */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = 1;                 /* base case */
        }
        else if ((i & 1) == 0) {          /* even n: a(n) = a(n/2) */
            cache[i] = cache[i >> 1];
        }
        else {                            /* odd n > 1 */
            seq_t sum = 1;                /* start with 1 as per definition */
            /* enumerate all proper divisors of i */
            for (idx_t d = 1; d * d <= i; ++d) {
                if (i % d == 0) {
                    if (d < i)          /* d is a proper divisor */
                        sum += cache[d];
                    idx_t e = i / d;      /* complementary divisor */
                    if (e != d && e < i)
                        sum += cache[e];
                }
            }
            cache[i] = sum;
        }
    }
    last = n;  /* update the last computed index */

    return cache[n];
}
