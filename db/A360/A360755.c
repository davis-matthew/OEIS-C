/*
 * A360755  Expansion of (1/2) * Sum_{k>0} (2 * x * (1 + x^k))^k.
 * https://oeis.org/A360755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 2^(d-1) * binomial(d, n/d-1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* The sequence is zero‐based for n=1, so no special handling for n<1 is needed.
       We simply sum over all positive divisors d of n. */

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t q1 = n / d;
            seq_t term1 = pow2(d - 1) * binomial(d, q1 - 1);
            sum += term1;

            if (d != q1) {          /* account for the paired divisor */
                idx_t d2 = q1;
                idx_t q2 = n / d2;  /* which is d */
                seq_t term2 = pow2(d2 - 1) * binomial(d2, q2 - 1);
                sum += term2;
            }
        }
    }
    return sum;
}
