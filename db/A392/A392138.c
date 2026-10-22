/*
 * A392138  a(n) = 7*a(n-1) + 2*7^n - 1, a(0) = 0.
 * https://oeis.org/A392138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*7^n - (7^n - 1)/6
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* 7^n */
    seq_t pow7 = seq_pow((seq_t)7, n);

    /* 2 * n * 7^n */
    seq_t term1 = (seq_t)2 * (seq_t)n * pow7;

    /* (7^n - 1) / 6 */
    seq_t term2 = (pow7 - (seq_t)1) / (seq_t)6;

    return term1 - term2;
}
