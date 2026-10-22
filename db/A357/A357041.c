/*
 * A357041  a(n) = Sum_{d|n} 2^(d-1) * binomial(d+n/d-1,d).
 * https://oeis.org/A357041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 2^(d-1) * binomial(d + n/d - 1, d)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the sum by iterating over all divisors d of n. */
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t q1 = n / d1;
            idx_t m1 = d1 + q1 - 1;
            seq_t term1 = pow_uint(2, d1 - 1) * binomial(m1, d1);
            total += term1;

            idx_t d2 = q1;
            if (d2 != d1) {
                idx_t q2 = n / d2;   /* equals d1 */
                idx_t m2 = d2 + q2 - 1;
                seq_t term2 = pow_uint(2, d2 - 1) * binomial(m2, d2);
                total += term2;
            }
        }
    }
    return total;
}
