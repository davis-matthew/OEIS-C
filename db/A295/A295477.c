/*
 * A295477  a(n) = (24*n)!*n!/((12*n)!*(8*n)!*(5*n)!).
 * https://oeis.org/A295477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(24*n,12*n) * binomial(12*n,4*n) /
 *            binomial(5*n,n)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case explicitly */
    if (n == 0)
        return (seq_t)1;

    /* Precompute the required arguments */
    idx_t twentyfour = 24 * n;
    idx_t twelve     = 12 * n;
    idx_t four       = 4 * n;
    idx_t five       = 5 * n;

    /* Compute the binomial coefficients */
    seq_t num1 = binomial(twentyfour, twelve);   /* (24n)! / (12n)! / (12n)! */
    seq_t num2 = binomial(twelve, four);         /* (12n)! / (8n)! / (4n)! */
    seq_t denom = binomial(five, n);             /* (5n)! / (n)! / (4n)! */

    /* Reduce the fraction to keep intermediate values small */
    seq_t g = gcd(num1, denom);
    if (g != 1) {
        num1 /= g;
        denom /= g;
    }

    g = gcd(num2, denom);
    if (g != 1) {
        num2 /= g;
        denom /= g;
    }

    /* At this point denom should be 1, but use it to be safe */
    return (num1 * num2) / denom;
}
