/*
 * A241894  The total number of squares and rectangles appearing in the
 *          Thue-Morse sequence (1, 0 version) logical matrices after n
 *          stages.
 * https://oeis.org/A241894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10 + 4·2ⁿ + 4^(n+1)) / 18 for even n a(n) = (4 + 8·2ⁿ +
 *            4^(n+1)) / 18 for odd n * (This follows from 18·a(n) = 7 + 6·2ⁿ
 *            + 4^(n+1) + (-1)ⁿ·(3 - 2^(n+1)).)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* 2ⁿ */
    seq_t pow2n = big_pow((seq_t)2, (seq_t)n);

    /* 4^(n+1) */
    seq_t pow4np1 = big_pow((seq_t)4, (seq_t)(n + 1));

    seq_t numerator;
    if ((n & 1) == 0) {          /* n even */
        numerator = (seq_t)10 + (seq_t)4 * pow2n + pow4np1;
    } else {                     /* n odd */
        numerator = (seq_t)4 + (seq_t)8 * pow2n + pow4np1;
    }

    /* divide by 18 – the result is always an integer */
    return numerator / (seq_t)18;
}
