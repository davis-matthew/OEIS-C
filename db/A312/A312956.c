/*
 * A312956  Coordination sequence Gal.5.264.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312956
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

/* A312956: Coordination sequence Gal.5.264.1
   Formula (recursive):  a(n) = 2*a(n-6) - a(n-12)   for n > 12
   Initial values:  a(0)=1, a(1)=4, a(2)=9, a(3)=13, a(4)=18, a(5)=24,
                    a(6)=26, a(7)=31, a(8)=36, a(9)=40, a(10)=45,
                    a(11)=52, a(12)=52                                               */

#include <stdlib.h>

/* Static cache of computed values */
static seq_t *cache = NULL;     /* array of sequence values              */
static size_t cap = 0;          /* allocated capacity (in seq_ts)        */
static idx_t computed_up_to = (idx_t)-1; /* highest index that has been computed */

/* The first 13 terms, as given. */
static const seq_t init_vals[13] = {
    1, 4, 9, 13, 18, 24, 26, 31, 36, 40, 45, 52, 52
};

/* Ensure that cache holds values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    /* Allocate or grow the cache if necessary */
    if (cache == NULL) {
        /* first time: at least enough for the initial terms or up to n */
        cap = (size_t)(n + 1);
        if (cap < 13) cap = 13;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (size_t i = 0; i < 13; ++i)
            cache[i] = init_vals[i];
        /* Zero remaining slots (unlikely but safe) */
        for (size_t i = 13; i < cap; ++i)
            cache[i] = 0;
        computed_up_to = 12;
    } else if (cap <= (size_t)n) {
        /* Need to grow: double until we can hold n+1 elements */
        size_t new_cap = cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        /* Zero the newly allocated space */
        for (size_t i = cap; i < new_cap; ++i)
            cache[i] = 0;
        cap = new_cap;
    }

    /* Compute missing values up to n */
    if (computed_up_to < n) {
        for (idx_t i = computed_up_to + 1; i <= n; ++i) {
            if (i < 13) {
                cache[(size_t)i] = init_vals[(size_t)i];
            } else {
                /* Recursive rule: a(i) = 2*a(i-6) - a(i-12) */
                cache[(size_t)i] = 2 * cache[(size_t)(i - 6)] - cache[(size_t)(i - 12)];
            }
        }
        computed_up_to = n;
    }
}

/* Public function returning a(n) */
seq_t a(idx_t n)
{
    /* n is unsigned, so no need to test negative values */
    ensure_cache(n);
    return cache[(size_t)n];
}
