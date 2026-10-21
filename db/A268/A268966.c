/*
 * A268966  Number of n X 3 0..2 arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling two not
 *          more than once.
 * https://oeis.org/A268966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n+2) * 3^(n-1) * (2 n + 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 2^(n+2) */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)(n + 2));
    /* Compute 3^(n-1) */
    seq_t pow3 = big_pow((seq_t)3, (seq_t)(n - 1));
    /* Compute (2 n + 1) */
    seq_t coeff = (seq_t)(2 * n + 1);
    /* Result is pow2 * pow3 * coeff */
    return ((pow2 * pow3) * coeff);
}
