/*
 * A294447  Lengths of square subwords (WW) of Kolakoski sequence A000002.
 * https://oeis.org/A294447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 2,4,2*3^(n-2) for n>=3 (n>=1)
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    seq_t exp = (seq_t)(n - 2);
    seq_t res = big_pow((seq_t)3, exp);
    res *= 2;
    return res;
}
