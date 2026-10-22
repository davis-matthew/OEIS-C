/*
 * A378733  G.f. A(x) satisfies A(x) = 1 + x / (1 - x*A(x)^2)^4.
 * https://oeis.org/A378733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binom(2*n-2*k+1, k) * binom(n+3*k-1, n-k) /
 *            (2*n-2*k+1) This is derived from the generating‑function A(x) =
 *            1 + x / (1 - x*A(x)^2)^4 (OEIS A378733). The term for n=0 is 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* a(0) = 1 */

    seq_t sum = 0;        /* accumulation of the sum */

    for (idx_t k = 1; k <= n; ++k)   /* k=0 term is 0 for n>0 */
    {
        /* First binomial: binom(2*n-2*k+1, k) */
        idx_t top1 = 2 * n - 2 * k + 1;
        if (top1 < k)                 /* binom is zero, skip */
            continue;
        seq_t bin1 = binomial(top1, k);

        /* Second binomial: binom(n+3*k-1, n-k) */
        idx_t top2   = n + 3 * k - 1;
        idx_t bottom = n - k;
        if (top2 < bottom)            /* binom is zero, skip */
            continue;
        seq_t bin2 = binomial(top2, bottom);

        /* Denominator: (2*n-2*k+1) = top1 */
        seq_t denom = (seq_t)top1;

        /* term = bin1 * bin2 / denom */
        seq_t term = bin1;
        term *= bin2;
        term /= denom;                /* exact division guaranteed */

        sum += term;
    }

    return sum;
}
