/*
 * A310516  Coordination sequence Gal.6.599.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + a(n-8) - a(n-10) for n > 11 Initial values:
 *            a(0)=1, a(1)=4, a(2)=10, a(3)=16, a(4)=22, a(5)=24, a(6)=32,
 *            a(7)=38, a(8)=42, a(9)=46, a(10)=54, a(11)=56
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Return the nth term of OEIS A310516. */
seq_t a(idx_t n)
{
    /* constant initial table (only used for n < 12) */
    static const seq_t init[12] = {
        1, 4, 10, 16, 22, 24, 32, 38, 42, 46, 54, 56
    };

    /* static cache of computed values, with the last computed index */
    static seq_t *cache = NULL;          /* pointer to allocated array */
    static size_t cache_len = 0;         /* allocated length of cache */
    static size_t cache_max = 0;         /* highest index that has been computed */

    /* handle small indices directly */
    if (n < 12)
        return init[(size_t)n];

    /* initialise cache on first use */
    if (cache == NULL) {
        cache_len = 12;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL)
            return 0; /* out of memory – sequence not needed if this happens */
        for (size_t i = 0; i < 12; ++i)
            cache[i] = init[i];
        cache_max = 11;
    }

    /* If n is already computed, just return it */
    if ((size_t)n <= cache_max)
        return cache[(size_t)n];

    /* Extend the cache if necessary */
    if ((size_t)n >= cache_len) {
        size_t new_len = cache_len * 2;
        if (new_len <= (size_t)n)
            new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL)
            return 0; /* out of memory */
        cache = tmp;
        cache_len = new_len;
        /* new slots are uninitialised; they will be filled below */
    }

    /* compute values from cache_max+1 up to n */
    for (size_t i = cache_max + 1; i <= (size_t)n; ++i) {
        seq_t a1 = cache[i - 2];
        seq_t a2 = cache[i - 8];
        seq_t a3 = cache[i - 10];
        cache[i] = a1 + a2 - a3;
    }

    cache_max = (size_t)n;
    return cache[(size_t)n];
}
