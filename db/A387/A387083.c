/*
 * A387083  Expansion of e.g.f. (2*(1-x)^2)/(2-4*x+x^2).
 * https://oeis.org/A387083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   2*a(n) - 4*n*a(n-1) + n*(n-1)*a(n-2) = 0
 * with initial values:
 *   a(0) = 1, a(1) = 0, a(2) = 1
 *
 * The function memoises previously computed values so it works for
 * calls in any order.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the trivial offsets directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t last_computed = 2;  /* we know a(0..2) already set */

    /* Expand cache if needed. */
    if (cache_size <= n) {
        idx_t new_size = cache_size ? cache_size << 1 : 8;
        while (new_size <= n) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        /* In case realloc failed, leave old cache untouched. */
        if (tmp) {
            cache = tmp;
            /* Initialise new part to zero */
            for (idx_t i = cache_size; i < new_size; ++i) cache[i] = (seq_t)0;
            cache_size = new_size;
            /* Set the known initial values. */
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)0;
            cache[2] = (seq_t)1;
        }
    }

    /* Compute forward from the last known value. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* term1 = 4 * i * a(i-1) */
        seq_t term1 = ((seq_t)4 * (seq_t)i) * cache[i - 1];
        /* term2 = i * (i-1) * a(i-2) */
        seq_t term2 = (seq_t)i * (seq_t)(i - 1) * cache[i - 2];
        /* a(i) = (term1 - term2) / 2 */
        seq_t val = (term1 - term2) / (seq_t)2;
        cache[i] = val;
    }
    last_computed = n;
    return cache[n];
}
