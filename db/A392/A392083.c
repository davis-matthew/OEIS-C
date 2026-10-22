/*
 * A392083  a(n) = 2*a(n-1) + 2^(n+1) - 1, a(0) = 0.
 * https://oeis.org/A392083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*2^(n+1) - 2^n + 1
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 26, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t p2n   = pow2(n);      /* 2^n   */
    seq_t p2n1  = pow2(n + 1);  /* 2^(n+1) */
    seq_t res   = (seq_t)n * p2n1; /* n * 2^(n+1) */
    res -= p2n;              /* subtract 2^n */
    res += 1;                /* add 1 */
    return res;
}
