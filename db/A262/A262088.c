/*
 * A262088  a(0)=0, a(1)=1, a(n) = -a(n-2)^2 - a(n-1)^3.
 * https://oeis.org/A262088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -a(n-2)^2 - a(n-1)^3, with a(0)=0, a(1)=1
 * Range:     i8 n <= 7, i16 n <= 8, i32 n <= 9, i64 n <= 9, i128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last = 1;

    /* initialize cache on first call */
    if (cache == NULL) {
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 1;
        last  = 1;
    }

    /* if requested value already computed, return it */
    if (n <= last)
        return cache[n];

    /* grow cache if needed */
    if (n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= n) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap   = new_cap;
    }

    /* compute values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t term = -(a2 * a2) - (a1 * a1 * a1);
        cache[i] = term;
    }
    last = n;
    return cache[n];
}
