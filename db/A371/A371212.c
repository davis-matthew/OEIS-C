/*
 * A371212  Number of ordered factorizations of n into factors > 4.
 * https://oeis.org/A371212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, n/d > 4} a(d), with a(1)=1 and
 *            a(2)=a(3)=a(4)=0. The function computes the sequence on demand,
 *            memoizing values in a static dynamic array
 * Range:     u8 n <= 2519, u16 n <= 9810, u32 n <= 9810, u64 n <= 9810,
 *            u128 n <= 9810, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for realloc, abort */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for out‑of‑bounds queries */

    /* static cache for memoization (1‑based indexing) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* allocated capacity (indices 0..cache_size-1) */
    static idx_t max_computed = 0;    /* largest n for which a[n] is finalized */

    /* enlarge cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;                    /* index 0 unused */
        seq_t *tmp = (seq_t *)realloc(cache,
                           (size_t)new_size * sizeof(seq_t));
        if (!tmp) abort();                         /* out of memory */
        cache = tmp;
        for (idx_t i = cache_size; i < new_size; ++i)
            cache[i] = 0;                          /* zero new entries */
        cache_size = new_size;
    }

    /* compute missing terms up to n */
    if (n > max_computed) {
        for (idx_t k = max_computed + 1; k <= n; ++k) {
            if (k == 1) {
                cache[1] = 1;
                continue;
            }
            if (k <= 4) {     /* a(2), a(3), a(4) = 0 */
                cache[k] = 0;
                continue;
            }
            seq_t sum = 0;
            idx_t limit = k / 5;   /* d must satisfy d <= k/5 */
            for (idx_t d = 1; d * d <= k; ++d) {
                if (k % d != 0) continue;
                if (d <= limit) sum += cache[d];
                idx_t other = k / d;
                if (other != d && other <= limit) sum += cache[other];
            }
            cache[k] = sum;
        }
        max_computed = n;
    }

    return cache[n];
}
