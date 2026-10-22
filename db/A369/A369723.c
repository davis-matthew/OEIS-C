/*
 * A369723  Expansion of e.g.f. exp( 2 * (1-sqrt(1-4*x)) ).
 * https://oeis.org/A369723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 4 a(n) = 2*(2*n-3)*a(n-1) + 16*a(n-2) for n >= 2
 *            (This recurrence is derived from the e.g.f.
 *            exp(2*(1-sqrt(1-4x))).) The function memoizes results so that
 *            calls in any order are fast
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoization table */
    static idx_t size  = 0;       /* current number of stored values (>=2 once initialized) */

    /* initialize cache for n = 0 and n = 1 */
    if (size == 0) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failed, return 0 as a fallback */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        size = 2;
    }

    /* if we already know the value, return it immediately */
    if (n < size) {
        return cache[n];
    }

    /* extend cache up to n */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return 0;                 /* allocation failed */
    cache = tmp;

    /* compute new entries using the recurrence */
    for (idx_t i = size; i <= n; ++i) {
        if (i == 0) { cache[0] = (seq_t)1; continue; }
        if (i == 1) { cache[1] = (seq_t)4; continue; }

        /* mult = 2*(2*i-3) as seq_t */
        seq_t mult = (seq_t)(2 * (2 * i - 3));
        /* term = mult * a(i-1) + 16 * a(i-2) */
        seq_t term = mult * cache[i - 1] + (seq_t)16 * cache[i - 2];
        cache[i] = term;
    }

    size = new_size;
    return cache[n];
}
