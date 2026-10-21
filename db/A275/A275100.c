/*
 * A275100  Number of set partitions of [3*n] such that within each block the
 *          numbers of elements from all residue classes modulo n are equal
 *          for n>0, a(0)=1.
 * https://oeis.org/A275100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^(n-1) + 3^n + 1 for n >= 1; a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t pow6 = seq_pow((seq_t)6, n - 1);   /* 6^(n-1)   */
    seq_t pow3 = seq_pow((seq_t)3, n);       /* 3^n       */
    return pow6 + pow3 + (seq_t)1;
}
