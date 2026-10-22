/*
 * A311101  Coordination sequence Gal.3.47.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-2) + 3*a(n-3) - a(n-4) for n > 8 *
 *            Initial values: * a(0)=1, a(1)=4, a(2)=8, a(3)=12, a(4)=15, *
 *            a(5)=19, a(6)=23, a(7)=28, a(8)=33
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* stored terms */
    static size_t cache_size = 0;        /* number of initialized terms */
    static size_t cache_cap   = 0;        /* allocated capacity */
    static const seq_t init[9] = {
        1, 4, 8, 12, 15, 19, 23, 28, 33
    };

    /* initialise cache on first call */
    if (cache_size == 0) {
        /* start with a capacity that can hold the first nine terms */
        cache_cap = 1;
        while (cache_cap < 9) cache_cap <<= 1;   /* 16 */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache)          /* allocation failure – unlikely for small sizes */
            return 0;
        for (size_t i = 0; i < 9; ++i)
            cache[i] = init[i];
        cache_size = 9;
    }

    /* if the requested index is already known, return it */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* enlarge cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)         /* allocation failure – should not happen */
            return 0;
        /* zero the newly allocated part */
        if (new_cap > cache_cap)
            memset(tmp + cache_cap, 0,
                   (new_cap - cache_cap) * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        long long val =
            3LL * (long long)cache[i - 1]
          - 4LL * (long long)cache[i - 2]
          + 3LL * (long long)cache[i - 3]
          - (long long)cache[i - 4];
        cache[i] = (seq_t)val;
    }
    cache_size = (size_t)n + 1;

    return cache[(size_t)n];
}
