/*
 * A291064  a(n) = 2^n*(n + 1) - 3*(n - 1).
 * https://oeis.org/A291064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n*(n + 1) - 3*(n - 1)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t pow2_n = pow2(n);
    seq_t term1   = pow2_n * (((seq_t)n) + 1);
    seq_t term2   = ((seq_t)3 * (((seq_t)n) - 1));
    return term1 - term2;
}
