/*
 * A364015  Expansion of Sum_{k>0} k * x^(2*k) / (1 + x^(3*k)).
 * https://oeis.org/A364015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n, n/d == 2 (mod 3)} (-1)^(n/d) * d
 * Range:     i8 n <= 207, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 is guaranteed by the OEIS offset (1) */
    seq_t res = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t k = n / d;             /* quotient n/d */
            /* contribution from divisor d */
            if (k % 3 == 2) {
                seq_t sign = (k & 1) ? -1 : 1;
                res += sign * (seq_t)d;
            }
            /* contribution from the complementary divisor k (= n/d) */
            if (d != k) {
                /* here quotient becomes d */
                if (d % 3 == 2) {
                    seq_t sign = (d & 1) ? -1 : 1;
                    res += sign * (seq_t)k;
                }
            }
        }
    }
    return res;
}
