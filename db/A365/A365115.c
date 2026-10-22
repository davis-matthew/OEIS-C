/*
 * A365115  G.f. satisfies A(x) = 1 + x / (1 - x*A(x))^5.
 * https://oeis.org/A365115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊(n+1)/2⌋} binomial(n-k+1, k) * binomial(n+4k-1,
 *            n-k) / (n-k+1) derived from A(x) = 1 + x/(1 - x*A(x))^5
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    idx_t maxk = (n + 1) / 2;              /* beyond this B1=0 */
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t b1 = binom(n - k + 1, k);    /* C(n-k+1, k) */
        if (!b1)
            continue;                     /* term is zero */

        /* b2 = C(n+4k-1, n-k) ; top is always ≥ bottom for k>0,
           for k==0 top=n-1 < bottom=n => b2=0. */
        idx_t top    = n + 4 * k - 1;
        idx_t bottom = n - k;
        seq_t b2 = (top < bottom) ? 0 : binom(top, bottom);
        if (!b2)
            continue;

        seq_t denom = n - k + 1;
        seq_t term  = b1 * b2;
        term /= denom;
        sum += term;
    }
    return sum;
}
