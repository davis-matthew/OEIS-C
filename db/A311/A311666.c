/*
 * A311666  Coordination sequence Gal.4.108.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A311666 – Coordination sequence Gal.4.108.1
 * Recurrence (valid for n > 15):
 *     a(n) = a(n-6) + a(n-7) - a(n-13)
 * Initial values (n = 0 … 23):
 *     1, 4, 8, 13, 14, 21, 28, 30, 38, 35, 40, 45, 52, 60, 64, 63,
 *     62, 71, 76, 84, 94, 89, 90, 93
 *
 * The function memoises computed values so that calls may arrive in any
 * order.  It uses dynamic allocation to grow the internal cache as
 * necessary.  The implementation is O(n) per new maximum n and runs in
 * well under 2 s for n ≲ 10 000.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t base_values[24] = {
    1, 4, 8, 13, 14, 21, 28, 30, 38, 35, 40, 45,
    52, 60, 64, 63, 62, 71, 76, 84, 94, 89, 90, 93
};

seq_t a(idx_t n)
{
    /* Local cache state --------------------------------------------------- */
    static seq_t *cache      = NULL;   /* array of computed a(i) values   */
    static size_t cache_cap  = 0;      /* current allocated size          */
    static idx_t  cache_max  = (idx_t)-1;   /* highest index computed     */

    /* --------------------------------------------------------------------- */
    /*   1.  For indices below 24 return the known value directly             */
    /* --------------------------------------------------------------------- */
    if (n < 24) {
        return base_values[(size_t)n];
    }

    /* --------------------------------------------------------------------- */
    /*   2.  Initialise cache on first use                                   */
    /* --------------------------------------------------------------------- */
    if (cache == NULL) {
        /* Allocate an initial capacity (reasonable small size) */
        cache_cap = 128;
        cache     = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out of memory – unlikely */

        /* Copy the known start of the sequence */
        for (size_t i = 0; i < 24; ++i) {
            cache[i] = base_values[i];
        }
        cache_max = 23;
    }

    /* --------------------------------------------------------------------- */
    /*   3.  Grow the cache so that it holds indices up to 'n'              */
    /* --------------------------------------------------------------------- */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 128;
        while (new_cap <= (size_t)n) {
            new_cap <<= 1;
        }
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;          /* out of memory – unlikely */
        cache     = new_cache;
        cache_cap = new_cap;
    }

    /* --------------------------------------------------------------------- */
    /*   4.  Compute missing terms using the recurrence                    */
    /* --------------------------------------------------------------------- */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        const size_t p6  = (size_t)(i - 6);
        const size_t p7  = (size_t)(i - 7);
        const size_t p13 = (size_t)(i - 13);
        cache[(size_t)i] = cache[p6] + cache[p7] - cache[p13];
    }
    cache_max = n;

    /* --------------------------------------------------------------------- */
    /*   5.  Return the requested term                                       */
    /* --------------------------------------------------------------------- */
    return cache[(size_t)n];
}
