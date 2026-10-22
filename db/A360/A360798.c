/*
 * A360798  Expansion of Sum_{k>0} x^k / (1 - (2 * x)^k)^(k+1).
 * https://oeis.org/A360798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 2^(n-d) * binomial(d+n/d-1,d)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 26, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* divisor d */
            seq_t p2   = pow2(n - d);
            seq_t bin  = binomial(d + n / d - 1, d);
            sum += p2 * bin;

            /* paired divisor n/d */
            idx_t d2 = n / d;
            if (d2 != d) {
                seq_t p22  = pow2(n - d2);
                seq_t bin2 = binomial(d2 + n / d2 - 1, d2);
                sum += p22 * bin2;
            }
        }
    }
    return sum;
}
