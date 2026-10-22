/*
 * A379190  G.f. A(x) satisfies A(x) = (1 + x*A(x)^3) * (1 + x*A(x))^3.
 * https://oeis.org/A379190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+2*k+1,k) *
 *            binomial(3*n+6*k+3,n-k)/(n+2*k+1) (OEIS A379190)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        /* binomial terms */
        seq_t b1 = binomial(n + 2*k + 1, k);
        seq_t b2 = binomial(3*n + 6*k + 3, n - k);
        /* denominator */
        seq_t denom = n + 2*k + 1;

        /* Reduce to avoid overflow: cancel gcd factors first */
        seq_t g1 = gcd(b1, denom);
        seq_t a1 = b1 / g1;
        seq_t rem = denom / g1;

        seq_t g2 = gcd(b2, rem);
        seq_t a2 = b2 / g2;
        rem = rem / g2;

        seq_t term = a1 * a2;
        if (rem != 0)
            term = term / rem;  /* rem should be 1 here */

        sum += term;
    }
    return sum;
}
