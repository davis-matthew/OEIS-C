/*
 * A243860  a(n) = 2^(n+1) - (n-1)^2.
 * https://oeis.org/A243860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) - (n-1)^2
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 2^(n+1) */
    seq_t val = pow2(n + 1);

    /* Compute (n-1)^2 with care for n==0 */
    seq_t sq;
    if (n == 0)
        sq = 1;          /* (-1)^2 */
    else {
        seq_t d = (seq_t)(n - 1);
        sq = d * d;
    }

    return val - sq;
}
