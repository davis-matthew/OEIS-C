/*
 * A314849  Coordination sequence Gal.6.246.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12) for n > 12 Initial values
 *            (0‑based): a(0) = 1, a(1) = 5, a(2) = 9, a(3) = 14, a(4) = 18,
 *            a(5) = 24, a(6) = 28, a(7) = 34, a(8) = 38, a(9) = 43, a(10) =
 *            47, a(11) = 52, a(12) = 57
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cached values of the sequence */
static size_t cache_size = 0;        /* number of cached entries */

/* Initialise the cache with the first 13 terms. */
static void init_cache(void)
{
    if (!cache) {
        cache_size = 13;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();        /* cannot proceed without memory */

        /* set the initial 13 values */
        cache[0]  = 1;
        cache[1]  = 5;
        cache[2]  = 9;
        cache[3]  = 14;
        cache[4]  = 18;
        cache[5]  = 24;
        cache[6]  = 28;
        cache[7]  = 34;
        cache[8]  = 38;
        cache[9]  = 43;
        cache[10] = 47;
        cache[11] = 52;
        cache[12] = 57;
    }
}

/* Compute the nth term of the sequence.  Results are cached, so each
   term is computed at most once, even if requested in arbitrary order. */
seq_t a(idx_t n)
{
    init_cache();

    size_t idx = (size_t)n;
    if (idx < cache_size)
        return cache[idx];

    /* grow the cache to hold indices up to n */
    size_t new_size = idx + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache)
        abort(); /* memory allocation failed */

    cache = new_cache;

    /* compute missing entries using the recurrence */
    for (size_t i = cache_size; i <= idx; ++i) {
        /* recurrence: a(i) = a(i-1) + a(i-11) - a(i-12) */
        cache[i] = cache[i-1] + cache[i-11] - cache[i-12];
    }

    cache_size = new_size;
    return cache[idx];
}
