/*
 * A356277  a(n) is the smallest integer that can be written as (3 + 1/t_1)^m
 *          * (3 + 1/t_2)^(n-m) with integers t_1,t_2 >= 2 and 0 < m < n.
 * https://oeis.org/A356277
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 10^(n/2) if n is even 2^5 * 10^((n-3)/2) if n is odd and
 *            n ≤ 5 5^5 * 10^((n-7)/2) if n is odd and n ≥ 7
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n < 2) 
        return 0;                   /* undefined for n < 2 */

    if ((n & 1) == 0)               /* even n */
        return seq_pow(10, n / 2);

    /* odd n */
    if (n <= 5)
    {
        seq_t part   = seq_pow(10, (n - 3) / 2);
        seq_t factor = seq_pow(2, 5);            /* 32 */
        return seq_mul(factor, part);
    }
    else
    {
        seq_t part   = seq_pow(10, (n - 7) / 2);
        seq_t factor = seq_pow(5, 5);            /* 3125 */
        return seq_mul(factor, part);
    }
}
