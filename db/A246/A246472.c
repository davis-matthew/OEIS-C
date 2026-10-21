/*
 * A246472  Number of order-preserving (monotone) functions from the power set
 *          of 1 = {0} to the power set of n = {0, ..., n-1}.
 * https://oeis.org/A246472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2*n-1) + 2^n - binomial(2*n, n)/2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the base case a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* pow2(2*n-1) is 2^(2*n-1) */
    seq_t pow_half = pow2(2 * n - 1);
    /* pow2(n) is 2^n */
    seq_t pow_n    = pow2(n);
    /* binomial(2*n, n) gives the middle binomial coefficient */
    seq_t bino     = binom(2 * n, n);
    /* The middle coef is even, so divide by 2 */
    seq_t half_bino = bino / (seq_t)2;

    /* Final formula */
    return pow_half + pow_n - half_bino;
}
