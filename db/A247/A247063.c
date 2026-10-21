/*
 * A247063  Dynamic Betting Game D(n,5,3).
 * https://oeis.org/A247063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-13) - a(n-14) (n ≥ 14) a(0) = 0; the values
 *            for 1 ≤ n ≤ 13 are: 1,2,3,4,5,6,8,9,10,11,12,13,16
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* seq_t and idx_t are provided by the build environment. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* a[0]..a[...], cache[0]=0, cache[1] = a(1) */
    static size_t cache_size = 0; /* number of elements allocated */
    static idx_t last_computed = 0; /* largest n for which cache[n] is valid */

    /* Prepare base values up to 13 if not done yet */
    if (cache_size == 0)
    {
        cache_size = 14;           /* indices 0..13 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)4;
        cache[5] = (seq_t)5;
        cache[6] = (seq_t)6;
        cache[7] = (seq_t)8;
        cache[8] = (seq_t)9;
        cache[9] = (seq_t)10;
        cache[10] = (seq_t)11;
        cache[11] = (seq_t)12;
        cache[12] = (seq_t)13;
        cache[13] = (seq_t)16;
        last_computed = 13;
    }

    /* Grow cache if required */
    if (n + 1 > cache_size)
    {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) /* allocation failure: fall back to 0 */
            return (seq_t)0;
        cache = new_cache;
        cache_size = new_size;
    }

    /* Compute missing terms up to n */
    if (n > last_computed)
    {
        for (idx_t i = last_computed + 1; i <= n; ++i)
        {
            if (i <= 13)
            {
                /* value already set in base block */
                continue;
            }
            /* recurrence: a(i) = a(i-1) + a(i-13) - a(i-14) */
            cache[i] = cache[i - 1] + cache[i - 13] - cache[i - 14];
        }
        last_computed = n;
    }

    return cache[n];
}
