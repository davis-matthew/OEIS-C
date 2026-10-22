/*
 * A301631  Numerator of population variance of n-th row of Pascal's triangle.
 * https://oeis.org/A301631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator( binomial(2n,n)/(n+1) - 4^n/(n+1)^2 )
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   binomial, gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = ( binomial(2n,n)*(n+1) - 4^n ) / (n+1)^2  */
#include "../utils/gcd.h"
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* (n+1) as seq_t */
    seq_t np1 = (seq_t)n + 1;

    /* binomial(2n, n) */
    seq_t bin = binomial((idx_t)(2 * n), (idx_t)n);

    /* 4^n  = 2^(2n)  */
    seq_t four_pow = pow2((idx_t)(2 * n));

    /* numerator before reduction : bin*(n+1) - 4^n */
    seq_t term1 = bin * np1;
    seq_t N = term1 - four_pow;

    /* denominator (n+1)^2 */
    seq_t denom = np1 * np1;

    /* reduce by gcd */
    seq_t g = gcd(N, denom);
    return N / g;
}
