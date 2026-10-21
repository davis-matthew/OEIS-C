/*
 * A289999  Sierpinski cuboctahedral numbers: a(n) = 16*4^n - 12*2^n + 9.
 * https://oeis.org/A289999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*4^n - 12*2^n + 9
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute 2^n and 2^(2n) using the fast exponentiation helper */
    idx_t two_exp = n << 1;          /* 2n */
    seq_t pow2_n   = pow2(n);        /* 2^n  */
    seq_t pow2_2n  = pow2(two_exp);  /* 2^(2n) = 4^n */

    /* a(n) = 16*4^n - 12*2^n + 9 = 16*2^(2n) - 12*2^n + 9 */
    seq_t result = 16 * pow2_2n - 12 * pow2_n + 9;

    return result;
}
