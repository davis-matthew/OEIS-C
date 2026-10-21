/*
 * A249453  a(0) = 4; for n>0, a(n) = a(n-1) + 2^n - 3.
 * https://oeis.org/A249453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 + 2^(n+1) - 3*n
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* use the closed‑form expression */
    seq_t pow = pow2(n + 1);          /* 2^(n+1) */
    seq_t sub = (seq_t)3 * n;         /* 3*n   */
    return (seq_t)2 + pow - sub;
}
