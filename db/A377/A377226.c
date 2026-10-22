/*
 * A377226  Take the sequence of the signed denominators of Leibniz series for
 *          Pi/4 (cf. A157142) and permute the terms so that a negative term
 *          follows every two positive terms and the absolute difference
 *          between two consecutive terms of the same sign is 4.
 * https://oeis.org/A377226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-3) - a(n-6) for n > 5 a(0) = 1, a(1) = 5, a(2) =
 *            -3, a(3) = 9, a(4) = 13, a(5) = -7 Sequence A377226 (computed
 *            with memoised recurrence)
 * Range:     i8 n <= 47, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t size = 0;

    /* if the value is already cached, return immediately */
    if (n < size)
        return cache[n];

    /* first time: allocate enough space (at least 6 terms) */
    if (size == 0) {
        size_t init = (size_t)((n >= 6) ? n + 1 : 6);
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        if (cache == NULL)
            return 0; /* out of memory – return 0 as fallback */

        /* initialise known terms */
        cache[0] = (seq_t) 1;
        cache[1] = (seq_t) 5;
        cache[2] = (seq_t)-3;
        cache[3] = (seq_t) 9;
        cache[4] = (seq_t)13;
        cache[5] = (seq_t)-7;
        size = 6;
    }
    /* expand cache if needed */
    if (n >= size) {
        size_t newsize = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (tmp == NULL)
            return 0; /* out of memory – fallback */
        cache = tmp;
        /* compute new terms using the recurrence */
        for (idx_t i = size; i <= n; ++i) {
            /* i >= 6 by construction, so indices exist */
            cache[i] = (seq_t) (2 * cache[i - 3] - cache[i - 6]);
        }
        size = n + 1;
    }

    return cache[n];
}
