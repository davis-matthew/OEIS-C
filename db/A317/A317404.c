/*
 * A317404  a(n) = 3*n*(2^n - 1).
 * https://oeis.org/A317404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n*(2^n - 1)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 25, u64 n <= 56, u128 n <= 119,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t two_n = pow2(n);          /* 2^n as seq_t */
    seq_t term  = two_n - 1;        /* (2^n - 1) */
    return ((seq_t)3) * (seq_t)n * term;
}
