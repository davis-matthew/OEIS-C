/*
 * A298788  Coordination sequence for bey tiling (or net) with respect to a
 *          trivalent node.
 * https://oeis.org/A298788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=3, a(2)=7, a(3)=12, a(4)=13, a(5)=17, a(6)=24; a(n)
 *            = 2*a(n-3) - a(n-6) for n > 6
 * Range:     u8 n <= 65, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>    /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Initial seven values of the sequence.  They are used
       directly for n < 7. */
    static const seq_t init[7] = {
        1, 3, 7, 12, 13, 17, 24
    };
    static seq_t *cache    = NULL;      /* array holding a[0] .. a[highest] */
    static size_t  cache_cap = 0;        /* capacity of the array */
    static idx_t   highest_computed = 6; /* last index that has been computed */

    /* trivial case */
    if (n < 7)
        return init[n];

    /* Allocate cache on the first call and copy the initial values. */
    if (cache == NULL) {
        cache_cap = 64;                    /* arbitrary starting size   */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)                 /* allocation failure        */
            return (seq_t)0;

        /* copy the seven known values */
        for (idx_t i = 0; i < 7; ++i)
            cache[i] = init[i];
    }

    /* Grow the cache if the requested index is out of bounds. */
    {
        size_t need = (size_t)n + 1;       /* we need indices 0..n      */
        if (need > cache_cap) {
            size_t new_cap = cache_cap ?: 1;
            while (new_cap < need)
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL)              /* realloc failure           */
                return (seq_t)0;
            cache = tmp;
            cache_cap = new_cap;
        }
    }

    /* Compute any missing values up to index n.  All the terms needed
       by the recurrence are guaranteed to exist (they are at most
       six positions behind). */
    for (idx_t i = highest_computed + 1; i <= n; ++i) {
        cache[i] = 2 * cache[i - 3] - cache[i - 6];
    }
    highest_computed = n;

    return cache[n];
}
