/*
 * A248049  a(n) = (a(n-1) + a(n-2)) * (a(n-2) + a(n-3)) / a(n-4) with a(0) =
 *          2, a(1) = a(2) = a(3) = 1.
 * https://oeis.org/A248049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1)+a(n-2)) * (a(n-2)+a(n-3)) / a(n-4) with a(0) = 2,
 *            a(1) = a(2) = a(3) = 1. The recurrence is evaluated with seq_t
 *            arithmetic only, so it works for arbitrary‑precision seq_t as
 *            well as for fixed‑size unsigned types
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 10, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first four indices directly. */
    if ((idx_t)0 == n) return (seq_t)2;
    if ((idx_t)1 == n || (idx_t)2 == n || (idx_t)3 == n) return (seq_t)1;

    /* Static cache of already computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t computed_up_to = 0;   /* largest index already computed */

    size_t target = (size_t)n;

    /* Initialise the cache for the first four terms. */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();          /* out‑of‑memory */
        cache[0] = (seq_t)2;
        cache[1] = cache[2] = cache[3] = (seq_t)1;
        computed_up_to = 3;
    }

    /* Grow the cache if necessary. */
    if (target + 1 > cache_cap) {
        size_t new_cap = target + 1;
        if (new_cap < cache_cap * 2) new_cap = cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to the requested one. */
    for (size_t i = computed_up_to + 1; i <= target; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];

        seq_t sum12 = a1 + a2;
        seq_t sum23 = a2 + a3;
        seq_t numer = sum12 * sum23;
        cache[i] = numer / a4;
    }

    computed_up_to = target;
    return cache[target];
}
