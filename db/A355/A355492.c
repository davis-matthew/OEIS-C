/*
 * A355492  a(n) = 7*3^n - 2.
 * https://oeis.org/A355492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*3^n - 2
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 3^n first, then multiply by 7 and subtract 2. */
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);
    seq_t res  = pow3 * ((seq_t)7);
    res -= (seq_t)2;
    return res;
}
