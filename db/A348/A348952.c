/*
 * A348952  a(n) = -Sum_{d|n, d < sqrt(n)} (-1)^(d + n/d).
 * https://oeis.org/A348952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -Sum_{d|n, d<sqrt(n)} (-1)^(d + n/d)
 * Range:     i8 n <= 9503, i16 n <= 9503, i32 n <= 9503, i64 n <= 9503,
 *            i128 n <= 9503, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d * d < n; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;
            seq_t val = ((d + e) & 1) ? -1 : 1;
            sum += val;
        }
    }
    return -sum;
}
