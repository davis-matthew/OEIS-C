/*
 * A315306  Coordination sequence Gal.5.130.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   for n > 6, a(n) = 2*a(n-1) - a(n-2) - a(n-4) + 2*a(n-5) - a(n-6)
 *            Initial values: a(0)=1, a(1)=6, a(2)=10, a(3)=16, a(4)=20,
 *            a(5)=24, a(6)=30 The sequence is computed by dynamic programming
 *            with memoisation
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base_vals[7] = {1, 6, 10, 16, 20, 24, 30};

static seq_t *cache = NULL;        /* array of computed values   */
static size_t cache_cap = 0;       /* allocated size             */
static idx_t  max_computed = (idx_t)-1;  /* highest index that has been computed */

/* Ensure that the cache can hold indices up to n.  Reallocate if needed. */
static void
ensure_cache(idx_t n)
{
    if (cache_cap > n)
        return;

    size_t new_cap = n + 1;               /* need space for 0..n */
    /* Grow by a factor of 2 at least for efficiency */
    if (new_cap < 64)
        new_cap = 64;
    if (cache_cap > 0 && new_cap < cache_cap * 2)
        new_cap = cache_cap * 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* In the unlikely event of allocation failure, abort. */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;

    /* Initialise new entries that fall inside the first 7 terms. */
    if (cache_cap >= 7) {
        for (idx_t i = 0; i < 7; ++i)
            cache[i] = base_vals[i];
    } else {
        /* For very small capacities, initialise what fits. */
        for (idx_t i = 0; i < cache_cap; ++i)
            cache[i] = base_vals[i];
    }
}

/* Compute values from (max_computed+1) up to n.  Assumes cache is large enough. */
static void
compute_up_to(idx_t n)
{
    if (n <= max_computed)
        return;

    idx_t start = (max_computed == (idx_t)-1) ? 0 : max_computed + 1;
    for (idx_t i = start; i <= n; ++i) {
        if (i <= 6) {
            /* base values already set */
            continue;
        }
        /* recurrence:
           a(i) = 2*a(i-1) - a(i-2) - a(i-4) + 2*a(i-5) - a(i-6)  */
        seq_t val = cache[i-1] + cache[i-1];       /* 2*a(i-1) */
        val -= cache[i-2];                         /* - a(i-2) */
        val -= cache[i-4];                         /* - a(i-4) */
        val += cache[i-5] + cache[i-5];            /* + 2*a(i-5) */
        val -= cache[i-6];                         /* - a(i-6) */
        cache[i] = val;
    }
    max_computed = n;
}

seq_t a(idx_t n)
{
    if (n <= 6)
        return base_vals[n];

    /* First time initialization */
    if (!cache) {
        ensure_cache(n);
        max_computed = 6; /* base values upto 6 are ready */
    } else {
        ensure_cache(n);
    }

    compute_up_to(n);
    return cache[n];
}
