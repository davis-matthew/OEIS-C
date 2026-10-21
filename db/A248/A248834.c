/*
 * A248834  The numerator of curvature of touching circles inscribed in a
 *          special way in the smaller segment of circle of radius 1/6 divided
 *          by a chord of length sqrt(8/75).
 * https://oeis.org/A248834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(0) = 15 a(1) = 25 a(2) = 245 a(3) = 3025 a(n) = 17*a(n-1) -
 *            51*a(n-2) + 27*a(n-3) for n > 3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cached values of the sequence */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms currently stored (>=0) */
    static idx_t cache_cap = 0;   /* allocated size of cache               */

    /* initialise the cache with the first four terms if necessary */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        cache[0] = 15;
        cache[1] = 25;
        cache[2] = 245;
        cache[3] = 3025;
        cache_len = 4;
    }

    /* if the requested index is already cached, return it immediately */
    if (n < cache_len)
        return cache[n];

    /* grow the cache to at least (n+1) elements */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute successive terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0)        val = 15;
        else if (i == 1)   val = 25;
        else if (i == 2)   val = 245;
        else if (i == 3)   val = 3025;
        else
            val = 17 * cache[i-1] - 51 * cache[i-2] + 27 * cache[i-3];
        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
