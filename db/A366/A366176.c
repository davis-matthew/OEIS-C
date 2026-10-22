/*
 * A366176  G.f. A(x) satisfies A(x) = 1 + x*A(x)^3/(1 - x)^2.
 * https://oeis.org/A366176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+k-1,n-k) *
 *            binomial(3*k,k)/(2*k+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* Compute the OEIS sequence A366176.
   Uses the direct summation formula. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                 /* a(0) = 1 */

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t c = binomial(n + k - 1, n - k);          /* binom(n+k-1,n-k) */
        seq_t b = binomial(3 * k, k);                  /* binom(3k,k) */
        b = b / (2 * k + 1);                           /* divide by (2k+1) */
        sum += c * b;                                  /* term addition */
    }
    return sum;
}
