/*
 * A396262  a(n) = Sum_{d|n, d < sqrt(n)} (-1)^(n/d) * d^2.
 * https://oeis.org/A396262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n, d < sqrt(n)} (-1)^(n/d) * d^2
 * Range:     i8 n <= 70, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* Loop over divisors d < sqrt(n) */
    for (idx_t d = 1; d * d < n; ++d) {
        if (n % d == 0) {
            seq_t term = (seq_t)d * (seq_t)d;   /* d^2 */
            if (((n / d) & 1ULL) != 0) {
                sum -= term;   /* odd exponent -> negative sign */
            } else {
                sum += term;   /* even exponent -> positive sign */
            }
        }
    }
    return sum;
}
