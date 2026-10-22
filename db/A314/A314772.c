/*
 * A314772  Coordination sequence Gal.4.126.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-5) - a(n-10) for n > 13 Initial values: a(0)=1,
 *            a(1)=5, a(2)=9, a(3)=13, a(4)=19, a(5)=26, a(6)=31, a(7)=35,
 *            a(8)=39, a(9)=45, a(10)=52, a(11)=57, a(12)=59, a(13)=63
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init[14] = {1,5,9,13,19,26,31,35,39,45,52,57,59,63};

seq_t a(idx_t n)
{
    /* static cache for values computed so far */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* allocated length */
    static idx_t  max_computed = 0; /* highest index currently stored */

    /* values for n < 14 are known directly */
    if (n < 14)
        return init[(size_t)n];

    /* ensure the cache can hold indices 0 .. n */
    size_t needed = (size_t)n + 1;
    if (cache_len < needed) {
        /* allocate or grow the cache */
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp)
            /* allocation failure – should not happen for n ≤ 10000 */
            return init[0];
        cache = tmp;
        cache_len = needed;

        /* initialise first 14 entries if they are not already set */
        if (max_computed < 13) {
            for (size_t i = 0; i < 14; ++i)
                cache[i] = init[i];
            max_computed = 13;
        }
    }

    /* compute any missing values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = ((seq_t)2 * cache[i - 5]) - cache[i - 10];
    }
    max_computed = n;
    return cache[n];
}
