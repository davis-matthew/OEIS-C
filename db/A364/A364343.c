/*
 * A364343  Expansion of Sum_{k>0} k * x^k/(1 + x^k)^3.
 * https://oeis.org/A364343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (n/2) * Sum_{d|n} (-1)^(d+1) * (d+1)
 * Range:     i8 n <= 11, i16 n <= 167, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1; safety for n==0 */

    seq_t sum = 0;

    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* process divisor i */
            int sign = ((i + 1) & 1) ? -1 : 1;
            seq_t term = (seq_t)(i + 1);
            if (sign < 0) term = -term;
            sum += term;

            /* process complementary divisor n/i if distinct */
            idx_t d2 = n / i;
            if (d2 != i) {
                sign = ((d2 + 1) & 1) ? -1 : 1;
                term = (seq_t)(d2 + 1);
                if (sign < 0) term = -term;
                sum += term;
            }
        }
    }

    seq_t result = ((seq_t)n) * sum / 2;
    return result;
}
