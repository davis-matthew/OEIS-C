/*
 * A254363  a(n) = 4^n + 6*2^n + 3^(n+1) + 10.
 * https://oeis.org/A254363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n + 6*2^n + 3^(n+1) + 10
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t res = seq_pow((seq_t)4, n);                /* 4^n      */
    res += ((seq_t)6) * seq_pow((seq_t)2, n);        /* 6*2^n    */
    res += seq_pow((seq_t)3, n + (idx_t)1);          /* 3^(n+1)  */
    res += (seq_t)10;                                /* +10      */
    return res;
}
