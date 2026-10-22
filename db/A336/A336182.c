/*
 * A336182  a(n) = Sum_{k=0..n} (-3)^k * binomial(n,k)^3.
 * https://oeis.org/A336182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-3)^k * binomial(n,k)^3
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 11, i64 n <= 21, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t len = 0;

    if (n < len)
        return cache[(size_t)n];

    if (cap < (size_t)(n + 1)) {
        cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        cap   = (size_t)(n + 1);
    }

    for (idx_t j = len; j <= n; ++j) {
        seq_t sum  = 0;
        seq_t binom = 1;
        seq_t pow   = 1;
        for (idx_t k = 0; k <= j; ++k) {
            seq_t term = pow * binom * binom * binom;
            sum += term;
            if (k == j)
                break;
            /* update binomial coefficient for next k */
            binom = binom * (seq_t)(j - k) / (seq_t)(k + 1);
            /* update (-3)^k */
            pow   = pow * (seq_t)(-3);
        }
        cache[(size_t)j] = sum;
    }
    len = n + 1;
    return cache[(size_t)n];
}
