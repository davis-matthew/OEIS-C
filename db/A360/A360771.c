/*
 * A360771  Expansion of Sum_{k>=0} (x * (2 + x^k))^k.
 * https://oeis.org/A360771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(0) = 1; for n>0: a(n) = Sum_{d|n} 2^(d - n/d + 1) *
 *            binomial(d, n/d - 1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Need functions for powers of two and binomial coefficients. */
#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* First divisor d1 = i */
            idx_t d1 = i;
            idx_t q1 = n / d1;        /* quotient = n/d1 */
            idx_t k1 = q1 - 1;        /* k = n/d - 1 */
            if (k1 <= d1) {
                seq_t bin1 = binomial(d1, k1);
                if (bin1) {
                    idx_t exp1 = d1 - q1 + 1;
                    sum = sum + pow2(exp1) * bin1;
                }
            }

            /* Second divisor d2 = n/i, if distinct */
            idx_t d2 = n / i;
            if (d2 != d1) {
                idx_t q2 = i;          /* quotient = n/d2 = i */
                idx_t k2 = q2 - 1;
                if (k2 <= d2) {
                    seq_t bin2 = binomial(d2, k2);
                    if (bin2) {
                        idx_t exp2 = d2 - q2 + 1;
                        sum = sum + pow2(exp2) * bin2;
                    }
                }
            }
        }
    }
    return sum;
}
