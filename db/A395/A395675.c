/*
 * A395675  Expansion of g^3/(2-g)^4, where g = 1+x*g^2 is the g.f. of
 *          A000108.
 * https://oeis.org/A395675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n+1)/2) * ( binomial(2*n+1,n) + 2^(2*n) )
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute the central binomial coefficient C(2*n+1, n) */
    seq_t bin = binom(2 * n + 1, n);

    /* Compute 2^(2*n) */
    seq_t pow = pow2(2 * n);

    /* Sum of the two terms */
    seq_t sum = bin + pow;

    /* (n+1) as a seq_t value */
    seq_t factor = (seq_t)(n + 1);

    /* Final formula: ((n+1)/2) * sum  */
    return (factor * sum) / 2;
}
