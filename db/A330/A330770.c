/*
 * A330770  a(n) = 19 * 8^n + 17 for n >= 0.
 * https://oeis.org/A330770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19 * 8^n + 17, n >= 0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 41,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"   /* provides seq_t big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    seq_t pow8   = big_pow((seq_t)8, (seq_t)n);
    seq_t result = 19 * pow8 + 17;
    return result;
}
