/*
 * A390561  Lower (1/5,1/2) midsequence of (2^n) and (5*n); see Comments.
 * https://oeis.org/A390561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( 2^n /5 + (5*n)/2 ) = (2^(n+1) + 25*n) / 10
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 30, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: lower (1/5,1/2) midsequence of (2^n) and (5*n), offset 0. */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* compute 2^(n+1) */
    seq_t two_pow_next = pow_uint(2, n + 1);   /* 2^(n+1) */

    /* compute 25 * n */
    seq_t part2 = (seq_t)25 * n;              /* 25*n */

    /* sum and divide by 10 (integer division = floor for non‑negative values) */
    seq_t sum   = two_pow_next + part2;
    return sum / (seq_t)10;
}
