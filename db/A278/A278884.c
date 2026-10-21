/*
 * A278884  a(n) = binomial(3*n-1,n) * binomial(3*n,n)/(2*n+1).
 * https://oeis.org/A278884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(3*n-1,n) * binomial(3*n,n) / (2*n+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Denominator for the final division */
    seq_t denom = (seq_t)(2 * n) + 1;

    /* Compute the two binomial coefficients */
    seq_t b1 = binomial((idx_t)(3 * n - 1), n);  /* C(3n‑1, n) */
    seq_t b2 = binomial((idx_t)(3 * n),     n);  /* C(3n,   n) */

    /* Cancel any common factor of the denominator with b1
       so that the product stays as small as possible.        */
    seq_t g = gcd(b1, denom);
    b1 /= g;
    denom /= g;

    /* After removing common factors from b1, the remaining
       part of the denominator must divide b2.                 */
    g = gcd(b2, denom);
    b2 /= g;
    denom /= g;

    /* At this point denom should be 1, otherwise
       the input data are inconsistent.  For safety we
       fall back to a generic computation.               */
    if (denom != (seq_t)1)
        return (b1 * b2) / ((seq_t)(2 * n) + 1);

    /* Final result: product of the reduced binomials. */
    return b1 * b2;
}
