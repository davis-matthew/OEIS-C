/*
 * A318063  Number of nX3 0..1 arrays with every element unequal to 1, 2, 3 or
 *          4 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A318063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 9*a(n-2) - 7*a(n-3) - 3*a(n-4) + 8*a(n-5) -
 *            2*a(n-6) - 9*a(n-7) - 2*a(n-8) - a(n-9) + 2*a(n-11) (n > 12)
 *            Initial values: a(1) = 1 a(2) = 18 a(3) = 98 a(4) = 649 a(5) =
 *            4003 a(6) = 25106 a(7) = 156846 a(8) = 980960 a(9) = 6133497
 *            a(10) = 38352691 a(11) = 239814461 a(12) = 1499536...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Precomputed first 12 values; required to bootstrap the recurrence. */
    static const seq_t base[12] =
    {
        (seq_t)1,                   /* a(1)  */
        (seq_t)18,                  /* a(2)  */
        (seq_t)98,                  /* a(3)  */
        (seq_t)649,                 /* a(4)  */
        (seq_t)4003,                /* a(5)  */
        (seq_t)25106,               /* a(6)  */
        (seq_t)156846,              /* a(7)  */
        (seq_t)980960,              /* a(8)  */
        (seq_t)6133497,             /* a(9)  */
        (seq_t)38352691,            /* a(10) */
        (seq_t)239814461,           /* a(11) */
        (seq_t)1499536299           /* a(12) */
    };

    /* Cache for previously computed terms.  The array is
       used to support arbitrary call order. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n <= 12)
        return base[n - 1];

    /* Grow the cache to contain at least n entries. */
    if (n > cache_size)
    {
        seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!new_cache)        /* handle allocation failure gracefully */
            return (seq_t)0;
        cache = new_cache;

        /* Initialise cached values up to 12, if not already done. */
        if (cache_size < 12)
        {
            for (idx_t i = 0; i < 12; ++i)
                cache[i] = base[i];
            cache_size = 12;
        }

        /* Compute new terms using the empirical recurrence. */
        for (idx_t i = cache_size; i < n; ++i)
        {   /* cache[i] corresponds to a(i+1) */
            seq_t sum = 5 * cache[i - 1]      /* a(n-1)   */
                      + 9 * cache[i - 2]      /* a(n-2)   */
                      + 8 * cache[i - 5]      /* a(n-5)   */
                      + 2 * cache[i - 11];    /* a(n-11)  */

            seq_t sub = 7 * cache[i - 3]      /* a(n-3)   */
                      + 3 * cache[i - 4]      /* a(n-4)   */
                      + 2 * cache[i - 6]      /* a(n-6)   */
                      + 9 * cache[i - 7]      /* a(n-7)   */
                      + 2 * cache[i - 8]      /* a(n-8)   */
                      + cache[i - 9];        /* a(n-9)   */

            cache[i] = sum - sub;            /* a(n)     */
        }
        cache_size = n;
    }

    return cache[n - 1];
}
