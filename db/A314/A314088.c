/*
 * A314088  Coordination sequence Gal.5.111.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-9) - a(n-10) for n > 10 initial values:
 *            a(0)=1, a(1)=5, a(2)=11, a(3)=16, a(4)=20, a(5)=24, a(6)=28,
 *            a(7)=33, a(8)=39, a(9)=44, a(10)=49
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* First 11 terms known a priori */
    static const seq_t init[11] = {
        (seq_t)1,  (seq_t)5,  (seq_t)11, (seq_t)16,
        (seq_t)20, (seq_t)24, (seq_t)28, (seq_t)33,
        (seq_t)39, (seq_t)44, (seq_t)49
    };

    /* Return immediately for the trivial cases */
    if (n < (idx_t)11) {
        return init[(size_t)n];
    }

    /* Memoisation table for all higher values */
    static seq_t *cache = NULL;        /* dynamic array a[i] */
    static size_t  cache_cap = 0;      /* allocated entries */
    static size_t  max_computed = 0;   /* largest i with a[i] ready */

    /* Initialise the table on the first call */
    if (cache == NULL) {
        cache_cap = 11;                      /* at least 0..10 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) {     /* very unlikely, but keep the reference */
            return init[10]; /* safest value for n>=11 */
        }
        for (size_t i = 0; i < 11; ++i) {
            cache[i] = init[i];
        }
        max_computed = 10;
    }

    /* Grow the cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < ((size_t)n + 1)) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed: fall back to recomputation */
            /* recompute n directly without memoisation */
            seq_t prev = init[10];
            seq_t n9 = init[2];
            seq_t n10 = init[1];
            for (idx_t i = 11; i <= n; ++i) {
                seq_t cur = prev + n9 - n10;
                /* shift for next iteration */
                n10 = n9;
                n9 = prev;
                prev = cur;
            }
            return prev;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing values up to n */
    for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
        /* recurrence: a(i) = a(i-1) + a(i-9) - a(i-10) */
        cache[i] = cache[i - 1] + cache[i - 9] - cache[i - 10];
    }
    max_computed = (size_t)n;

    return cache[(size_t)n];
}
