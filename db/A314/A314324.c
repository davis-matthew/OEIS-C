/*
 * A314324  Coordination sequence Gal.5.252.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-4) - a(n-8) for n > 16 Initial values (a(0) …
 *            a(16)): 1, 5, 7, 13, 17, 26, 25, 34, 37, 48, 43, 56, 57, 70, 59,
 *            78, 75
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t init_vals[17] = {
    1, 5, 7, 13, 17, 26, 25, 34, 37, 48, 43, 56, 57, 70, 59, 78, 75
};

static seq_t *cache       = NULL;   /* allocated array for values   */
static idx_t  cache_cap   = 0;       /* current allocation size      */
static idx_t  cache_len   = 0;       /* highest index computed + 1   */

/* Ensure that the cache has at least (n+1) capacity */
static void ensure_capacity(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;
    idx_t new_cap = n + 1;
    if (new_cap < 32)
        new_cap = 32;           /* small default for first grow   */
    if (new_cap < 2 * cache_cap)
        new_cap = 2 * cache_cap;
    seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
    if (!tmp) {
        /* allocation failed – terminate program (cannot recover) */
        exit(1);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the cache with the first 17 known values */
static void init_cache(void)
{
    if (cache)
        return;
    ensure_capacity(16);          /* guarantee space for indices 0..16 */
    for (idx_t i = 0; i < 17; ++i)
        cache[i] = init_vals[i];
    cache_len = 17;
}

/* Compute a(n) with memoisation and the recurrence relation */
seq_t a(idx_t n)
{
    init_cache();

    if (n < cache_len)
        return cache[n];

    /* Extend the cache if necessary */
    ensure_capacity(n);

    /* Fill in values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 17) {
            cache[i] = init_vals[i];
        } else {
            /* a(i) = 2*a(i-4) - a(i-8)   for i > 16 */
            cache[i] = 2 * cache[i - 4] - cache[i - 8];
        }
    }
    cache_len = n + 1;
    return cache[n];
}
