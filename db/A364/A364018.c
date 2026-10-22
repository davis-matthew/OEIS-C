/*
 * A364018  Expansion of Sum_{k>0} (-1)^(k-1) * x^(3*k-1) / (1 - x^(3*k-1))^3.
 * https://oeis.org/A364018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n, n/d==2 (mod 3)} (-1)^(n/d) * binomial(d+1, 2)
 * Range:     i8 n <= 31, i16 n <= 495, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* out of offset – return zero safely */

    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            /* process divisor d1 */
            idx_t k1 = d2;            /* k = n/d1 */
            if (k1 % 3 == 2) {
                seq_t dd = (seq_t)d1;
                seq_t bin = dd * (dd + 1) / 2;   /* C(d1+1,2) */
                if (k1 & 1) bin = -bin;         /* (-1)^k1 */
                sum += bin;
            }

            if (d1 != d2) {
                /* process divisor d2 */
                idx_t d = d2;
                idx_t k = n / d;             /* k = n/d = i */
                if (k % 3 == 2) {
                    seq_t dd = (seq_t)d;
                    seq_t bin = dd * (dd + 1) / 2;
                    if (k & 1) bin = -bin;
                    sum += bin;
                }
            }
        }
    }
    return sum;
}
