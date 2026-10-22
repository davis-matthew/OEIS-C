/*
 * A305623  Number of chiral pairs of rows of n colors with exactly 3
 *          different colors.
 * https://oeis.org/A305623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 * (S2(n,3) - S2(ceil(n/2),3))
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   mul_small, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Compute S2(n,3) = (3^n - 3*2^n + 3) / 6 */
    seq_t two   = pow_uint(2, n);
    seq_t three = pow_uint(3, n);
    seq_t s2_n  = (three - mul_small(two, 3) + 3) / 6;

    /* Compute S2(ceil(n/2),3) */
    idx_t m = (n + 1) / 2;
    seq_t two_m   = pow_uint(2, m);
    seq_t three_m = pow_uint(3, m);
    seq_t s2_m    = (three_m - mul_small(two_m, 3) + 3) / 6;

    /* Final result */
    seq_t diff    = s2_n - s2_m;
    return mul_small(diff, 3);
}
