/*
 * A315303  Coordination sequence Gal.6.247.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315303
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
 * Formula: a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5)
 *                - 2*a(n-6) + 2*a(n-7) - 2*a(n-8) + 2*a(n-9) - a(n-10)
 *                for n > 10; a(0) = 1, a(1) = 6, a(2) = 10, a(3) = 16,
 *                a(4) = 20, a(5) = 24, a(6) = 28, a(7) = 32, a(8) = 38,
 *                a(9) = 42, a(10) = 48.
 */

#include <stdlib.h>

/* initial 11 terms */
static const seq_t init_vals[11] = {
    (seq_t)1, (seq_t)6, (seq_t)10, (seq_t)16, (seq_t)20,
    (seq_t)24, (seq_t)28, (seq_t)32, (seq_t)38, (seq_t)42, (seq_t)48
};

/* cache for memoisation */
static seq_t *cache = NULL;
static size_t  cache_cap = 0;   /* allocated capacity of cache */
static size_t  cache_len = 0;   /* number of valid terms currently stored */

/* ensure cache can hold indices up to n (inclusive) */
static void ensure_cache_capacity(idx_t n)
{
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (cache_cap == 0) ? 1 : cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* extend cache if necessary */
    ensure_cache_capacity(n);

    /* compute terms from cache_len up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 11) {
            cache[i] = init_vals[i];
        } else {
            /* linear recurrence of order 10 */
            seq_t val =
                2 * cache[i-1] - 2 * cache[i-2] + 2 * cache[i-3] - 2 * cache[i-4] + 2 * cache[i-5]
                - 2 * cache[i-6] + 2 * cache[i-7] - 2 * cache[i-8] + 2 * cache[i-9] - cache[i-10];
            cache[i] = val;
        }
    }
    /* update stored length */
    cache_len = (size_t)n + 1;
    return cache[n];
}
