/*
 * A350921  a(0) = 3, a(1) = 3, and a(n) = 6*a(n-1) - a(n-2) - 4 for n >= 2.
 * https://oeis.org/A350921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2) - 4, a(0) = 3, a(1) = 3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to support non‑increasing calls */
    static seq_t *cache = (seq_t *)NULL;
    static idx_t cached_up_to = 0;

    /* Initialize cache with first two terms */
    if (cache == (seq_t *)NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (cache == (seq_t *)NULL) return (seq_t)0; /* allocation failed */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)3;
        cached_up_to = 1;
    }

    if (n <= cached_up_to)
        return cache[n];

    /* Grow cache to accommodate terms up to n */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == (seq_t *)NULL) return (seq_t)0; /* allocation failed */
    cache = tmp;

    /* Compute missing terms */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        seq_t a_prev  = cache[i - 1];
        seq_t a_prev2 = cache[i - 2];
        cache[i] = 6 * a_prev - a_prev2 - 4;
    }

    cached_up_to = n;
    return cache[n];
}
