/*
 * A394523  E.g.f. A(x) satisfies A(x) = 1/(1 - 4*x*A(x)^3)^(1/4).
 * https://oeis.org/A394523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = \prod_{k=0}^{n-1} (7*n+1 - 4*k) / (7*n+1) Derived from
 *            a(n) = 4ⁿ·n!·binomial((7n+1)/4,n)/(7n+1) which simplifies to the
 *            above integer product
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 (empty product) */
    if (n == 0)
        return (seq_t)1;

    /* Keep the original denominator 7*n+1 for the product terms. */
    seq_t denom_n = (seq_t)7 * (seq_t)n + (seq_t)1;

    /* Remaining denominator to be cancelled progressively. */
    seq_t denom_rem = denom_n;

    seq_t prod = (seq_t)1;

    for (idx_t k = 0; k < n; ++k)
    {
        /* Current term of the product: (7*n+1 - 4*k) */
        seq_t term = denom_n - (seq_t)(4 * k);

        /* Cancel any common factor between this term and the remaining denominator. */
        seq_t g = gcd(term, denom_rem);
        if (g > 1)
        {
            term   /= g;
            denom_rem /= g;
        }

        prod *= term;
    }

    /* After processing all terms denom_rem must be 1. */
    return prod;
}
