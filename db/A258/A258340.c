/*
 * A258340  a(n) = (7^n + 3^n - 2)/8.
 * https://oeis.org/A258340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (7^n + 3^n - 2)/8
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t p7 = big_pow((seq_t)7, (seq_t)n);
    seq_t p3 = big_pow((seq_t)3, (seq_t)n);
    seq_t sum = p7 + p3 - (seq_t)2;
    return sum / (seq_t)8;
}
