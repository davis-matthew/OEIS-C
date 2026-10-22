/*
 * A365118  G.f. satisfies A(x) = (1 + x / (1 - x*A(x)))^2.
 * https://oeis.org/A365118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*(n-k+1),k) * binomial(n-1, n-k) /
 *            (n-k+1) with the convention a(0)=1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        idx_t m = n - k + 1;                 /* n-k+1  */
        seq_t b1 = binomial(2 * m, k);      /* binom(2m, k)   */
        seq_t b2 = binomial(n - 1, n - k);  /* binom(n-1, n-k) */
        seq_t term = b1 * b2;
        term /= m;                            /* exact division */
        sum += term;
    }
    return sum;
}
