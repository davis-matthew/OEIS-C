/*
 * A371211  Number of ordered factorizations of n into factors > 3.
 * https://oeis.org/A371211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, n/d > 3} a(d) with a(1) = 1
 * Range:     u8 n <= 1439, u16 n <= 9706, u32 n <= 9706, u64 n <= 9706,
 *            u128 n <= 9706, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* table of a(i), 1‑based, stored at cache[i-1] */
    static idx_t cacheSize   = 0;   /* allocated capacity */
    static idx_t maxComputed = 0;   /* largest n for which cache[i] is valid */

    if (n == 0)
        return 0;   /* undefined for n < 1, return 0 safely */

    /* enlarge cache if needed */
    if (n > cacheSize) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp && n > 0)
            return 0;  /* allocation failure – return 0 as a safe fallback */
        cache = tmp;
        cacheSize = n;
    }

    /* compute missing values up to n */
    if (n > maxComputed) {
        /* ensure a(1) is set before we start */
        if (maxComputed == 0) {
            cache[0] = (seq_t)1;
            maxComputed = 1;
        }

        for (idx_t m = maxComputed + 1; m <= n; ++m) {
            seq_t sum = 0;

            for (idx_t d = 1; d * d <= m; ++d) {
                if (m % d == 0) {
                    idx_t c = m / d;

                    /* divisor d */
                    if (c > 3)
                        sum += cache[d - 1];

                    /* divisor c (only if distinct from d) */
                    if (d != c && d > 3)
                        sum += cache[c - 1];
                }
            }
            cache[m - 1] = sum;
        }
        maxComputed = n;
    }

    return cache[n - 1];
}
