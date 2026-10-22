/*
 * A315045  Coordination sequence Gal.6.366.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315045
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
 * Formula: a(n) = 2*a(n-6) - a(n-12) for n > 17;
 * Initial values:
 * a(0)=1, a(1)=5, a(2)=9, a(3)=15, a(4)=20, a(5)=23,
 * a(6)=29, a(7)=34, a(8)=39, a(9)=45, a(10)=50, a(11)=52,
 * a(12)=59, a(13)=62, a(14)=69, a(15)=75, a(16)=80, a(17)=80.
 *
 * The function caches computed values so that repeated or out‑of‑order
 * requests are answered in O(1), while the first computation up to n
 * costs O(n).  The recurrence is stable and yields non‑negative
 * values, so unsigned arithmetic is safe.
 */

#include <stdlib.h>

static const seq_t init_vals[18] = {
    1, 5, 9, 15, 20, 23, 29, 34, 39, 45,
    50, 52, 59, 62, 69, 75, 80, 80
};

/* Cached sequence values.  Initially NULL. */
static seq_t *cache = NULL;
/* Current capacity of the cache array (number of elements). */
static size_t cache_cap = 0;
/* Largest index that has been computed so far. */
static idx_t max_comp = 17;

/* Ensure that the cache has room for indices up to `need`. */
static void ensure_capacity(idx_t need)
{
    if ((size_t)need <= cache_cap)
        return;

    /* Grow capacity exponentially. */
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap <= (size_t)need)
        new_cap *= 2;

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        return; /* Allocation failed – leave cache unchanged. */

    /* Copy existing values. */
    if (cache) {
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }

    /* Initialize new slots with the hard‑coded initial values while
       they are still in the 0..17 range.  Remaining slots stay zero
       and will be computed on demand. */
    for (size_t i = cache_cap; i < new_cap; ++i) {
        if (i < 18)
            new_cache[i] = init_vals[i];
        else
            new_cache[i] = 0;
    }

    cache      = new_cache;
    cache_cap  = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 18)                 /* easy: return one of the constants */
        return init_vals[n];

    /* Grow the cache if necessary. */
    ensure_capacity(n);

    /* Compute any missing values up to n. */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        if (i < 18)            /* safety check – should not happen */
            cache[i] = init_vals[i];
        else
            cache[i] = (2 * cache[i - 6]) - cache[i - 12];
    }
    max_comp = n;

    return cache[n];
}
