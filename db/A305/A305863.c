/*
 * A305863  a(n) = 6144*5^n - 12288*4^n + 7616*3^n - 1472*2^n + 41.
 * https://oeis.org/A305863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6144*5^n - 12288*4^n + 7616*3^n - 1472*2^n + 41
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 8, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t pow5 = big_pow((seq_t)5, (seq_t)n);
    seq_t pow4 = big_pow((seq_t)4, (seq_t)n);
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);

    seq_t pos = ((seq_t)6144 * pow5) + ((seq_t)7616 * pow3) + (seq_t)41;
    seq_t neg = ((seq_t)12288 * pow4) + ((seq_t)1472 * pow2);

    return pos - neg;
}
