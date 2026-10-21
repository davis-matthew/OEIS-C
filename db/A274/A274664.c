/*
 * A274664  Sum of n-th powers of the roots of x^3 + 11*x^2 - 4*x - 1.
 * https://oeis.org/A274664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -11*a(n-1) + 4*a(n-2) + a(n-3); a(0) = 3, a(1) = -11,
 *            a(2) = 129
 * Range:     i8 n <= 1, i16 n <= 4, i32 n <= 8, i64 n <= 17, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cached_len = 0;

    /* Return cached value if already computed */
    if ((size_t)n < cached_len)
        return cache[n];

    /* Grow cache to store up to index n */
    size_t new_len = (size_t)n + 1;
    if (cache == NULL) {
        cache = (seq_t *)malloc(new_len * sizeof(seq_t));
    } else {
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    }
    if (cache == NULL)  /* allocation failed – fallback */
        return (seq_t)0;

    /* Compute values from cached_len up to n */
    for (size_t i = cached_len; i <= new_len - 1; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)3;
        } else if (i == 1) {
            cache[1] = (seq_t)-11;
        } else if (i == 2) {
            cache[2] = (seq_t)129;
        } else {
            cache[i] = (seq_t)-11 * cache[i - 1] + (seq_t)4 * cache[i - 2] + cache[i - 3];
        }
    }
    cached_len = new_len;
    return cache[n];
}
