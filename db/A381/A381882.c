/*
 * A381882  Expansion of (1/x) * Series_Reversion( x / ((1+x)^3 * C(x)) ),
 *          where C(x) is the g.f. of A000108.
 * https://oeis.org/A381882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0..n} binomial(n+2k+1,k) * binomial(3n+3,n-k) /
 *            (n+2k+1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A381882 via the summation formula, reducing intermediate
 * products by cancelling the denominator with gcds to avoid overflow
 * in the 64‑bit build. */
#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        /* individual binomial terms */
        seq_t b1 = binomial(n + 2*k + 1, k);
        seq_t b2 = binomial(3*n + 3, n - k);
        /* denominator of the summand */
        seq_t denom = n + 2*k + 1;

        /* cancel denominator with first factor */
        seq_t g = gcd(b1, denom);
        b1 /= g;
        denom /= g;

        /* multiply remaining factor */
        seq_t term = b1 * b2;

        /* cancel any remaining denominator part */
        g = gcd(term, denom);
        term /= g;
        denom /= g;

        /* denom should now be 1 */
        sum += term;
    }
    return sum;
}
