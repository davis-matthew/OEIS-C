/*
 * A394903  Number of square grid edges that are visible through the n-th
 *          order Sierpinski carpet fractal.
 * https://oeis.org/A394903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (10*9^n - 12*8^n + 2*3^n)/5
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t nine_pow  = big_pow((seq_t)9,  (seq_t)n);
    seq_t eight_pow = big_pow((seq_t)8,  (seq_t)n);
    seq_t three_pow = big_pow((seq_t)3,  (seq_t)n);

    seq_t num = 10 * nine_pow - 12 * eight_pow + 2 * three_pow;
    seq_t res = num / (seq_t)5;
    return res;
}
