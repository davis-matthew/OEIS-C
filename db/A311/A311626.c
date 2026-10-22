/*
 * A311626  Coordination sequence Gal.6.232.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12) for n > 12 (a(0) = 1, a(1) =
 *            4, a(2) = 8, a(3) = 12, a(4) = 18, a(5) = 24, a(6) = 28, a(7) =
 *            34, a(8) = 40, a(9) = 44, a(10) = 48, a(11) = 52, a(12) = 56)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static const seq_t base_vals[13] = {
    1, 4, 8, 12, 18, 24, 28, 34, 40, 44, 48, 52, 56
};

static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of valid elements in cache */
static size_t cache_cap = 0;  /* allocated capacity */

static void ensure_capacity(idx_t n)
{
    if (n < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= (size_t)n)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(EXIT_FAILURE);   /* out of memory */
    if (cache) {
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        free(cache);
    } else {
        /* first time: copy base values */
        memcpy(new_cache, base_vals, sizeof(base_vals));
        cache_len = 13;
    }
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (!cache)          /* initialise cache with first 13 terms */
    {
        ensure_capacity(12);
        /* already filled in ensure_capacity */
    }

    if (n >= cache_len)
    {
        ensure_capacity(n);
        for (idx_t i = cache_len; i <= n; ++i)
        {
            /* recurrence holds for i >= 13 */
            cache[i] = cache[i-1] + cache[i-11] - cache[i-12];
        }
        cache_len = n + 1;
    }
    return cache[n];
}
