/*
 * A246360  a(1) = 1, then A007051 ((3^n)+1)/2 interleaved with A057198
 *          (5*3^(n-1)+1)/2.
 * https://oeis.org/A246360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; a(2n)=(3^n+1)/2; a(2n+1)=(5*3^(n-1)+1)/2
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 40, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    if ((n & 1) == 0)   /* even */
    {
        idx_t k = n / 2;
        seq_t pow = big_pow((seq_t)3, (seq_t)k); /* 3^k */
        return (pow + 1) / 2;
    }
    else                /* odd */
    {
        idx_t k = n / 2;                     /* floor(n/2) = (n-1)/2 */
        seq_t pow = big_pow((seq_t)3, (seq_t)(k - 1)); /* 3^(k-1) */
        return ((seq_t)5 * pow + 1) / 2;
    }
}
