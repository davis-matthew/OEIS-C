/*
 * A260921  Number of (n+2) X (2+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00010101 00101011 or 01010101.
 * https://oeis.org/A260921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 32, u64 n <= 70, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-2) + 6*a(n-4) + 4*a(n-6) - a(n-8) for n>10
   Initial values (n=1..10):
   a(1)=48, a(2)=68, a(3)=146, a(4)=279, a(5)=468, a(6)=831,
   a(7)=1528, a(8)=2750, a(9)=4874, a(10)=8781 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;

    if (n == 0)
        return (seq_t)0;          /* undefined, but avoid UB */

    /* initialise the cache with known terms up to n=10 */
    if (cache == NULL) {
        cache = (seq_t *)malloc((10 + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* memory failure: return 0 */

        cache[0] = (seq_t)0;               /* unused sentinel */
        cache[1] = (seq_t)48;              /* a(1) */
        cache[2] = (seq_t)68;              /* a(2) */
        cache[3] = (seq_t)146;             /* a(3) */
        cache[4] = (seq_t)279;             /* a(4) */
        cache[5] = (seq_t)468;             /* a(5) */
        cache[6] = (seq_t)831;             /* a(6) */
        cache[7] = (seq_t)1528;            /* a(7) */
        cache[8] = (seq_t)2750;            /* a(8) */
        cache[9] = (seq_t)4874;            /* a(9) */
        cache[10] = (seq_t)8781;           /* a(10) */
        max_cached = 10;
    }

    if (n <= max_cached)
        return cache[n];

    /* grow the cache up to n */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache)
        return (seq_t)0;   /* allocation failure */

    cache = new_cache;

    for (idx_t i = max_cached + 1; i <= n; ++i) {
        /* all indices i-2, i-4, i-6, i-8 are valid because i>=11 */
        seq_t val2 = cache[i - 2];
        seq_t val4 = cache[i - 4];
        seq_t val6 = cache[i - 6];
        seq_t val8 = cache[i - 8];

        seq_t sum = val2
                  + (seq_t)6 * val4
                  + (seq_t)4 * val6;

        cache[i] = sum - val8;   /* recurrence, guaranteed non‑negative */
    }

    max_cached = n;
    return cache[n];
}
