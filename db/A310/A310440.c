/*
 * A310440  Coordination sequence Gal.6.202.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310440
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
   OEIS A310440: Coordination sequence Gal.6.202.5
   Recurrence: a(n) = a(n-1) - a(n-2) + 2*a(n-3) - a(n-4) + a(n-5) - a(n-6)
   Initial values (n=0..6): 1, 4, 10, 15, 19, 24, 29
*/

#include <stdlib.h>   /* malloc, realloc */

static const seq_t INIT_VALS[7] = { 1, 4, 10, 15, 19, 24, 29 };

static seq_t *cache = NULL;      /* cached sequence values */
static size_t cache_cap = 0;     /* allocated capacity (number of elements) */
static idx_t   cache_last = 0;   /* largest index whose value is stored in cache */

/* Compute A310440 term a(n) for any n >= 0. */
seq_t a(idx_t n)
{
    /* Handle the small indices directly. */
    if (n < 7)
        return INIT_VALS[(size_t)n];

    /* Allocate or expand the cache if necessary. */
    if (cache == NULL) {
        cache_cap = (size_t)(n + 1);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            exit(1);                          /* allocation failure */
        /* Initialize the first seven terms. */
        for (size_t i = 0; i < 7; ++i)
            cache[i] = INIT_VALS[i];
        cache_last = 6;
    } else if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(1);
        cache = tmp;
        cache_cap = new_cap;
        /* Newly allocated slots are uninitialized; we will compute them later. */
    }

    /* If the requested value is already cached, return it. */
    if (n <= cache_last)
        return cache[(size_t)n];

    /* Compute any missing terms up to n. */
    idx_t start = (cache_last + 1 > 6) ? (cache_last + 1) : 7;
    for (idx_t i = start; i <= n; ++i) {
        __int128 sum = (__int128)cache[i - 1]
                     - (__int128)cache[i - 2]
                     + ( (__int128)cache[i - 3] << 1 )
                     - (__int128)cache[i - 4]
                     + (__int128)cache[i - 5]
                     - (__int128)cache[i - 6];
        cache[i] = (seq_t)sum;
    }
    cache_last = n;
    return cache[(size_t)n];
}
