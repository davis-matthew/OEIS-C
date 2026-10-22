/*
 * A375318  Expansion of 1/(1 - x^3*(1 + x)^4).
 * https://oeis.org/A375318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-3)+4*a(n-4)+6*a(n-5)+4*a(n-6)+a(n-7); Initial values:
 *            a(0)=1, a(1)=0, a(2)=0, a(3)=1, a(4)=4, a(5)=6, a(6)=5
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0; /* number of elements allocated in cache */

    idx_t needed = n + 1;
    if (needed > cache_size)
    {
        size_t new_sz = (size_t)needed;
        seq_t *tmp = (seq_t*)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failed */
        cache = tmp;

        /* initialise base values if not yet done */
        if (cache_size <= 6)
        {
            if (needed > 0) cache[0] = (seq_t)1;
            if (needed > 1) cache[1] = (seq_t)0;
            if (needed > 2) cache[2] = (seq_t)0;
            if (needed > 3) cache[3] = (seq_t)1;
            if (needed > 4) cache[4] = (seq_t)4;
            if (needed > 5) cache[5] = (seq_t)6;
            if (needed > 6) cache[6] = (seq_t)5;
        }

        /* compute missing terms using the recurrence */
        idx_t start = cache_size;
        if (start < 7) start = 7; /* recurrence applicable from n = 7 */
        for (idx_t i = start; i <= n; ++i)
        {
            cache[i] =
                cache[i-3] + (seq_t)4 * cache[i-4] + (seq_t)6 * cache[i-5]
                + (seq_t)4 * cache[i-6] + cache[i-7];
        }
        cache_size = needed;
    }
    return cache[n];
}
