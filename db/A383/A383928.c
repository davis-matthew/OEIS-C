/*
 * A383928  Expansion of g.f. cosh(9*arctanh(4*sqrt(x))).
 * https://oeis.org/A383928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n * (105 + 400*n + 3392*n^2 + 512*n^3 + 4096*n^4) *
 *            (2*n)! / (105 * (n!)^2) Equivalent: a(n) = 4^n * Poly(n) *
 *            C(2n,n) / 105 where Poly(n) = 105 + 400*n + 3392*n^2 + 512*n^3 +
 *            4096*n^4. The implementation computes the central binomial
 *            coefficient, reduces the division by 105 using a gcd...
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* polynomial in n */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n2 * n2;

    seq_t poly = 4096 * n4;
    poly += 512 * n3;
    poly += 3392 * n2;
    poly += 400 * nn;
    poly += 105;

    /* 4^n */
    seq_t pow4 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow4 *= 4;

    /* central binomial coefficient C(2n,n) */
    seq_t bin = binomial(2 * n, n);

    /* Reduce the division by 105 by cancelling any common factor
       with the binomial coefficient.  This keeps the intermediate
       division exact. */
    seq_t g = gcd(bin, (seq_t)105);
    seq_t bin_red = bin / g;
    unsigned denom = 105 / (unsigned)g;   /* denom ≤ 105 */

    /* Compute the product in an unsigned 128‑bit accumulator.
       The intermediate values for any n whose result fits in
       64 bits are guaranteed to fit in 128 bits. */
    unsigned __int128 tmp = (unsigned __int128)pow4 * (unsigned __int128)poly;
    tmp = tmp * (unsigned __int128)bin_red;
    tmp /= denom;                          /* exact division */

    return (seq_t)tmp;
}
