/*
 * A290396  a(n) = 3*2^n + 3*4^n + 6^(n+1) + 1.
 * https://oeis.org/A290396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^n + 3*4^n + 6^(n+1) + 1
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute powers using the arbitrary‑precision big_pow routine. */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);           /* 2^n   */
    seq_t pow4 = big_pow((seq_t)4, (seq_t)n);           /* 4^n   */
    seq_t pow6 = big_pow((seq_t)6, (seq_t)(n + 1));     /* 6^(n+1) */

    /* Assemble the final value.  All arithmetic is done in seq_t space. */
    return (seq_t)3 * pow2 + (seq_t)3 * pow4 + pow6 + (seq_t)1;
}
