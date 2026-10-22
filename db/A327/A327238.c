/*
 * A327238  Expansion of Sum_{k>=1} ((1 + k * x^k)^k - 1).
 * https://oeis.org/A327238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Σ_{d|n} (n/d)^d * binomial(n/d, d)
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 76, u64 n <= 215, u128 n <= 650,
 *            big any n
 * Helpers:   big_pow, binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, but safe */

    seq_t total = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* divisor d */
            idx_t m1 = n / d;                         /* n/d */
            seq_t pow1 = big_pow((seq_t)m1, (seq_t)d);/* (n/d)^d */
            seq_t bin1 = binomial(m1, d);             /* binomial(n/d, d) */
            seq_t term1 = seq_mul(pow1, bin1);        /* product */
            total = total + term1;

            /* paired divisor n/d, if different */
            idx_t d2 = n / d;
            if (d2 != d) {
                idx_t m2 = n / d2;                   /* = d */
                seq_t pow2 = big_pow((seq_t)m2, (seq_t)d2);/* (n/(n/d))^d2 = d^d2 */
                seq_t bin2 = binomial(m2, d2);        /* binomial(d, n/d) */
                seq_t term2 = seq_mul(pow2, bin2);
                total = total + term2;
            }
        }
    }
    return total;
}
