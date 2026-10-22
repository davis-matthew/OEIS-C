/*
 * A364593  G.f. satisfies A(x) = 1/(1-x) + x^2*(1-x)*A(x)^3.
 * https://oeis.org/A364593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} binomial(n-k, k) * binomial(3k, k) /
 *            (2k+1)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 23, u64 n <= 30, u128 n <= 30,
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
    for (idx_t k = 0; k <= n / 2; ++k) {
        seq_t b1 = binomial(n - k, k);
        seq_t b2 = binomial(3 * k, k);

        seq_t d = (seq_t)(2 * k + 1);

        seq_t g = gcd(b1, d);
        if (g > 1) {
            b1 /= g;
            d   /= g;
        }
        g = gcd(b2, d);
        if (g > 1) {
            b2 /= g;
            d   /= g;
        }

        /* After eliminating common factors, d should be 1. */
        seq_t term = b1 * b2;
        if (d > 1)   /* safety check, normally d == 1 */
            term /= d;

        sum += term;
    }
    return sum;
}
