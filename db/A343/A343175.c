/*
 * A343175  a(0)=2; for n > 0, a(n) = 2^(2*n-1) + 2^n + 1.
 * https://oeis.org/A343175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 2; for n > 0, a(n) = 2^(2*n-1) + 2^n + 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;

    /* Compute the two powers of two separately */
    seq_t p1 = pow2((idx_t)(2 * n - 1)); /* 2^(2*n-1) */
    seq_t p2 = pow2(n);                   /* 2^n */

    /* Sum according to the formula */
    return p1 + p2 + (seq_t)1;
}
