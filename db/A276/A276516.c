/*
 * A276516  Expansion of Product_{k>=1} (1-x^(k^2)).
 * https://oeis.org/A276516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = coefficient of q^n in ∏_{k≥1} (1 - q^{k^2})
 * Range:     i8 n <= 1937, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterative DP over square sizes, memoized across calls. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;
    static size_t buf_size = 0;

    size_t N = (size_t)n;
    if (N + 1 > buf_size) {
        buf_size = N + 1;
        buf = (seq_t *)realloc(buf, buf_size * sizeof(seq_t));
    }

    /* initialise */
    buf[0] = (seq_t)1;
    for (size_t i = 1; i <= N; ++i) buf[i] = (seq_t)0;

    /* process each square */
    for (size_t k = 1; ; ++k) {
        uint64_t sq64 = (uint64_t)k * (uint64_t)k;
        if (sq64 > (uint64_t)n) break;
        size_t s = (size_t)sq64;
        for (size_t i = N; ; --i) {
            buf[i] -= buf[i - s];
            if (i == s) break;
        }
    }
    return buf[N];
}
