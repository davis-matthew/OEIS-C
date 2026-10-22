/*
 * A379255  G.f. A(x) satisfies A(x) = ( (1 + x*A(x))/(1 - x*A(x)^3) )^3.
 * https://oeis.org/A379255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} binomial(3*n+7*k+2,k) *
 *            binomial(3*n+6*k+3,n-k) / (n+2*k+1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 0; a(0) = 1 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        /* first binomial: binomial(3*n + 7*k + 2, k) */
        idx_t n1 = 3 * n + 7 * k + 2;
        seq_t b1 = binomial(n1, k);

        /* second binomial: binomial(3*n + 6*k + 3, n - k) */
        idx_t n2 = 3 * n + 6 * k + 3;
        seq_t b2 = binomial(n2, n - k);

        /* product and division */
        seq_t prod = b1 * b2;
        seq_t denom = (seq_t)(n + 2 * k + 1);
        seq_t term = prod / denom;

        sum += term;
    }
    return sum;
}
