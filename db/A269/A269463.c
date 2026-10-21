/*
 * A269463  Number of length-n 0..4 arrays with no repeated value equal to the
 *          previous repeated value.
 * https://oeis.org/A269463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 4*a(n-2) - 32*a(n-3) Initial values: a(1)=5,
 *            a(2)=25, a(3)=120
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* out of range, defined as 0 */

    /* Static cache of computed terms, 1-indexed. */
    static seq_t *cache = NULL;
    static idx_t  cache_last = 0;    /* largest n cached */
    static size_t cache_cap  = 0;    /* allocated elements */

    /* First‑time initialization. */
    if (cache == NULL) {
        cache_cap = 8;               /* modest initial capacity */
        cache     = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;        /* allocation failure */

        cache[0] = (seq_t)0;
        /* base terms for n = 1, 2, 3 */
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)25;
        cache[3] = (seq_t)120;
        cache_last = 3;
    }

    /* Grow the cache if necessary. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;      /* double until enough */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                              /* reallocation failure */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    if (n > cache_last) {
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            seq_t val = 7 * cache[i - 1]
                      - 4 * cache[i - 2]
                      - 32 * cache[i - 3];
            cache[i] = val;
        }
        cache_last = n;
    }

    return cache[n];
}
