/*
 * A290707  a(n) = 2^(n+1) + n^2 - 1.
 * https://oeis.org/A290707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) + n^2 - 1
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t two_pow = pow2(n + 1);          /* 2^(n+1) */
    seq_t n_sq    = (seq_t)n * ((seq_t)n); /* n^2 */
    return two_pow + n_sq - 1;
}
