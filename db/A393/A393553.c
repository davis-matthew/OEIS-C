/*
 * A393553  Number of dominating sets in K_{2n,2n,2n} minus a perfect
 *          matching.
 * https://oeis.org/A393553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4^n-1)^3 + 3*(4^n-1)^2 - 3*n*(4^n-n-1) + 3
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 4^n */
    seq_t pow4n = big_pow((seq_t)4, (seq_t)n);

    /* t = 4^n - 1 */
    seq_t t = pow4n - (seq_t)1;

    /* t^2 and t^3 */
    seq_t t2 = t * t;
    seq_t t3 = t2 * t;

    /* u = 4^n - n - 1 */
    seq_t temp = pow4n - (seq_t)n;
    seq_t u = temp - (seq_t)1;

    /* 3 * n * u */
    seq_t group2 = (seq_t)3 * (seq_t)n * u;

    /* Final result: t3 + 3*t2 - group2 + 3 */
    seq_t result = t3 + (seq_t)3 * t2 - group2 + (seq_t)3;

    return result;
}
