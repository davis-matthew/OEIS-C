/*
 * A271744  Number of set partitions of [n] such that 5 is the largest element
 *          of the last block.
 * https://oeis.org/A271744
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 + 13*2^(n-5) + 7*3^(n-5) + 4^(n-5) for n>5; a(5) = 30.
 *            Computes the A271744 sequence at index n
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 20, u64 n <= 36, u128 n <= 68,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 5)
        return (seq_t)0;          /* undefined below offset */
    if (n == 5)
        return (seq_t)30;

    idx_t e = n - 5;
    seq_t pow2 = big_pow((seq_t)2, (seq_t)e);
    seq_t pow3 = big_pow((seq_t)3, (seq_t)e);
    seq_t pow4 = big_pow((seq_t)4, (seq_t)e);

    seq_t result = (seq_t)8;
    result += (seq_t)13 * pow2;
    result += (seq_t)7 * pow3;
    result += pow4;
    return result;
}
