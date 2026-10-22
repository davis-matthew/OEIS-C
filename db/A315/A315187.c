/*
 * A315187  Coordination sequence Gal.3.14.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-5) - a(n-6) for n > 6, with a(0)=1, a(1)=6,
 *            a(2)=10, a(3)=14, a(4)=18, a(5)=24, a(6)=30
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_cap = 0;

/* Ensure that cache contains values 0 … n. */
static void ensure_cache(idx_t n)
{
    idx_t required = n + 1;
    if (required < 7) required = 7;   /* always keep the first 7 terms */

    if (cache == NULL) {
        /* First time: allocate and fill base values. */
        cache = (seq_t *)malloc(required * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)10;
        cache[3] = (seq_t)14;
        cache[4] = (seq_t)18;
        cache[5] = (seq_t)24;
        cache[6] = (seq_t)30;
        cache_cap = 7;

        /* Compute any higher values up to the requested size. */
        for (idx_t i = 7; i < required; ++i)
            cache[i] = cache[i-1] + cache[i-5] - cache[i-6];
        cache_cap = required;
    } else if (cache_cap <= n) {
        /* Grow the cache and compute the missing terms. */
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;

        for (idx_t i = cache_cap; i < new_cap; ++i)
            cache[i] = cache[i-1] + cache[i-5] - cache[i-6];

        cache_cap = new_cap;
    }
}

/* Return the n-th term of A315187. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
