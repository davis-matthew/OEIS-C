/*
 * A304517  a(n) = 16*2^n - 11 (n>=1).
 * https://oeis.org/A304517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*2^n - 11
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1.  Compute 16*2^n - 11. */
    seq_t val = pow2(n);   /* 2^n */
    val = val * 16;        /* 16*2^n */
    return val - 11;       /* 16*2^n - 11 */
}
