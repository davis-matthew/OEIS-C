/*
 * A269278  Number of 3 X n 0..3 arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling three
 *          exactly once.
 * https://oeis.org/A269278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence for n > 7: a(n) = 36*a(n‑1) - 378*a(n‑2) + 972*a(n‑3)
 *            - 729*a(n‑4) with initial values a(1) = 0, a(2) = 1620, a(3) =
 *            46872, a(4) = 1104264, a(5) = 23549400, a(6) = 474819408, a(7) =
 *            9230973192
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache holds previously computed values, index 0 unused */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* size allocated (indices 0 .. cache_len-1) */

    /* initialize cache on first call */
    if (cache_len == 0)
    {
        cache_len = 8;                     /* need indices 0..7 at least */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        /* set initial values */
        cache[0] = 0;  /* unused */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1620;
        cache[3] = (seq_t)46872;
        cache[4] = (seq_t)1104264;
        cache[5] = (seq_t)23549400;
        cache[6] = (seq_t)474819408;
        cache[7] = (seq_t)9230973192;
    }

    if (n < 1)
        return (seq_t)0;          /* undefined for n<1, return 0 as safe default */

    /* if requested index is beyond current cache, enlarge and compute */
    if ((idx_t)cache_len <= n)
    {
        size_t new_len = (size_t)(n + 1);   /* indices 0 .. n inclusive */
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        /* compute values from old length up to n */
        for (size_t i = cache_len; i <= n; ++i)
        {
            seq_t term = 36 * cache[i-1];
            term -= 378 * cache[i-2];
            term += 972 * cache[i-3];
            term -= 729 * cache[i-4];
            cache[i] = term;
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
