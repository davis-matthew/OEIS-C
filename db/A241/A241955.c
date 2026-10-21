/*
 * A241955  a(n) = 2^(4*n+3) - 1.
 * https://oeis.org/A241955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(4*n+3) - 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t exp = (seq_t)n * 4 + 3;   /* 4*n + 3 */
    seq_t val = big_pow((seq_t)2, exp);  /* 2^(4*n+3) */
    return val - 1;
}
