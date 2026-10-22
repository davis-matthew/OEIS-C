/*
 * A353029  Number of copies of the star graph S(2,2,2) contained in the
 *          n-dimensional hypercube graph.
 * https://oeis.org/A353029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * C(n,3) * ((n-1)^3 - 3*(n-1))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 13, u64 n <= 35, u128 n <= 91,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;

    seq_t N = (seq_t)n;

    /* Compute binomial coefficient C(n,3) = n*(n-1)*(n-2)/6 */
    seq_t binom = N * (N - 1) * (N - 2);
    binom /= (seq_t)6;

    /* Compute the cubic factor ((n-1)^3 - 3*(n-1)) */
    seq_t nminus1 = N - 1;
    seq_t inner = nminus1 * nminus1 * nminus1 - (seq_t)3 * nminus1;

    /* Compute 2^n */
    seq_t pow2n = pow2(n);

    return pow2n * binom * inner;
}
