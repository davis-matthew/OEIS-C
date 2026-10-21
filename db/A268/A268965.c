/*
 * A268965  Number of n X 2 0..2 arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling two not
 *          more than once.
 * https://oeis.org/A268965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^(n-1) * (6*n+3)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t exp   = (seq_t)(n - 1);
    seq_t pow4  = big_pow((seq_t)4, exp);
    seq_t factor = (seq_t)(6 * (seq_t)n + 3);
    return pow4 * factor;
}
