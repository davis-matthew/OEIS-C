/*
 * A354294  Number of palindromic compositions of 2*n into parts <= n.
 * https://oeis.org/A354294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - 2^floor((n-1)/2), a(0) = 1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                     /* a(0) = 1 */

    seq_t big   = pow2(n);                   /* 2^n */
    idx_t small_exp = (n - 1) >> 1;          /* floor((n-1)/2) */
    seq_t small   = pow2(small_exp);         /* 2^floor((n-1)/2) */

    return big - small;                      /* a(n) */
}
