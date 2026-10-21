/*
 * A257289  a(n) = 8*9^n - 7*8^n.
 * https://oeis.org/A257289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*9^n - 7*8^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t p9 = seq_pow(9, n);
    seq_t p8 = seq_pow(8, n);
    return 8 * p9 - 7 * p8;
}
