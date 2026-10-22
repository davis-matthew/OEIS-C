/*
 * A310458  Coordination sequence Gal.4.78.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310458
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

/* Recurrence: a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4)
 *               + 2*a(n-5) - 2*a(n-6) + 2*a(n-7) - a(n-8)   (n > 8)
 * Initial values:
 *   a(0)=1, a(1)=4, a(2)=10, a(3)=15, a(4)=20, a(5)=25, a(6)=30,
 *   a(7)=36, a(8)=40
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* First nine terms are fixed. */
    static const seq_t init[9] = {1,4,10,15,20,25,30,36,40};

    /* For indices below 9 just return the hard‑coded value. */
    if (n < 9)
        return init[(size_t)n];

    /* Lazy, growing cache of computed terms. */
    static seq_t *cache = NULL;          /* array of terms */
    static size_t capacity = 0;          /* allocated size of array */
    static size_t max_computed = (size_t)-1;  /* largest index computed */

    /* Initialise the cache on first use. */
    if (!cache) {
        capacity = 9;                     /* enough for indices 0..8 */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return 0;             /* allocation failed */
        for (size_t i = 0; i < 9; ++i)
            cache[i] = init[i];
        max_computed = 8;                 /* last computed index */
    }

    size_t target = (size_t)n;

    /* If the requested index is already computed, just return it. */
    if (target <= max_computed)
        return cache[target];

    /* Grow the array if necessary to hold up to target. */
    if (target + 1 > capacity) {
        size_t newcap = capacity > 0 ? capacity : 9;   /* start from 9 for safety */
        while (newcap <= target)
            newcap <<= 1;           /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;         /* allocation failed */
        cache = tmp;
        capacity = newcap;
    }

    /* Compute missing terms up to target. */
    for (size_t i = max_computed + 1; i <= target; ++i) {
        cache[i] =
            2*cache[i-1] - 2*cache[i-2] + 2*cache[i-3] -
            2*cache[i-4] + 2*cache[i-5] - 2*cache[i-6] +
            2*cache[i-7] - cache[i-8];
    }

    max_computed = target;
    return cache[target];
}
