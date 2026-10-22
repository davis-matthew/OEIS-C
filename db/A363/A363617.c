/*
 * A363617  Expansion of Sum_{k>0} x^(3*k)/(1+x^k)^4.
 * https://oeis.org/A363617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -∑_{d|n} (-1)^d · binomial(d,3)
 * Range:     i8 n <= 10, i16 n <= 59, i32 n <= 1775, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* handle n >= 1; if called with n == 0 the loop does nothing */
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {   /* iterate over divisors up to √n safely */
        if (n % d == 0) {
            /* process divisor d */
            if (d >= 3) {
                seq_t dd = (seq_t)d;
                seq_t bin = dd * (dd - 1) * (dd - 2) / 6;
                int sign = (d % 2 == 0) ? 1 : -1;   /* (-1)^d */
                sum += (seq_t)sign * bin;
            }

            /* process complementary divisor e = n / d, distinct from d */
            idx_t e = n / d;
            if (e != d && e >= 3) {
                seq_t ee = (seq_t)e;
                seq_t bin = ee * (ee - 1) * (ee - 2) / 6;
                int sign = (e % 2 == 0) ? 1 : -1;
                sum += (seq_t)sign * bin;
            }
        }
    }
    return -sum;   /* a(n) = -∑_{d|n} (-1)^d · C(d,3) */
}
