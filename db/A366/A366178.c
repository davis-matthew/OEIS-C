/*
 * A366178  G.f. A(x) satisfies A(x) = 1/(1 - x) + x*A(x)^3/(1 - x)^3.
 * https://oeis.org/A366178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+4*k,n-k) *
 *            binomial(3*k,k)/(2*k+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 20, u128 n <= 20,
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
        /* binomial(n+4k, n-k) */
        idx_t m1 = n + 4 * k;
        idx_t r1 = n - k;
        seq_t t1 = binomial(m1, r1);

        /* binomial(3k, k) */
        idx_t m2 = 3 * k;
        seq_t t2 = binomial(m2, k);

        /* denominator 2k+1 */
        seq_t denom = (seq_t)(2 * k + 1);

        /* cancel common factors with denom to avoid overflow */
        seq_t g = gcd(t1, denom);
        t1 /= g;
        denom /= g;

        g = gcd(t2, denom);
        t2 /= g;
        denom /= g;

        /* after cancelation denom should be 1 */
        seq_t term = t1 * t2;
        if (denom != 1)
            term /= denom;   /* safety for non‑arbitrary builds */

        sum += term;
    }
    return sum;
}
