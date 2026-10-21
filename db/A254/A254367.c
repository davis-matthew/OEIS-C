/*
 * A254367  a(n) = 5*2^(n+2) + 2^(2n+2) + 10*3^n + 5^n + 35.
 * https://oeis.org/A254367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*2^(n+2) + 2^(2n+2) + 10*3^n + 5^n + 35
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* 5 * 2^(n+2) */
    seq_t pow2_n2 = seq_pow((seq_t)2, n + 2);
    seq_t term1   = pow2_n2 * 5;

    /* 2^(2n+2) */
    seq_t term2   = seq_pow((seq_t)2, 2 * n + 2);

    /* 10 * 3^n */
    seq_t term3   = seq_pow((seq_t)3, n);
    term3        *= 10;

    /* 5^n */
    seq_t term4   = seq_pow((seq_t)5, n);

    /* Sum all terms and the constant 35 */
    return term1 + term2 + term3 + term4 + (seq_t)35;
}
