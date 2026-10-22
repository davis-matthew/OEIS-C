/*
 * A364626  G.f. satisfies A(x) = 1/(1-x)^3 + x^2*A(x)^3.
 * https://oeis.org/A364626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = Sum_{k=0..floor(n/2)} binomial(n+4*k+2,6*k+2) * *
 *            binomial(3*k,k) / (2*k+1)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 2; ++k) {
        idx_t n1   = n + 4 * k + 2;
        idx_t m1   = 6 * k + 2;
        seq_t bin1 = binomial(n1, m1);
        seq_t bin2 = binomial(3 * k, k);

        /* binomial(3*k,k) is always divisible by (2*k+1) */
        seq_t denom = (seq_t)(2 * k + 1);
        seq_t term_coef = bin2 / denom;

        sum += bin1 * term_coef;
    }
    return sum;
}
