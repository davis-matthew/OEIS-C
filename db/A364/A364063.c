/*
 * A364063  Expansion of Sum_{k>0} k * x^k / (1 - x^(2*k-1)).
 * https://oeis.org/A364063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (sum of divisors of (2*n-1) + number of divisors of
 *            (2*n-1))/2
 * Range:     u8 n <= 156, u16 n <= 9141, u32 n <= 9141, u64 n <= 9141,
 *            u128 n <= 9141, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t m = 2 * n - 1;          /* m = 2n-1  (odd integer) */
    seq_t sigma = 1;              /* product of divisor sums */
    seq_t d_cnt  = 1;             /* product of (exponent+1) */

    idx_t tmp = m;                /* remaining part to factor */

    for (idx_t p = 2; p <= tmp / p; ++p)           /* trial division */
    {
        if (tmp % p == 0)
        {
            idx_t e = 0;
            seq_t sum_p = 1;   /* p^0 + p^1 + ... + p^e  (initially 1) */
            seq_t p_pow = 1;   /* current power of p */

            while (tmp % p == 0)          /* extract factor p^e */
            {
                tmp /= p;
                ++e;
                p_pow *= (seq_t)p;
                sum_p += p_pow;    /* add p^e to sum */
            }

            sigma *= sum_p;            /* update divisor sum product */
            d_cnt *= (seq_t)(e + 1);    /* update divisor count product */
        }
    }

    if (tmp > 1)                     /* remaining prime factor (exponent 1) */
    {
        sigma *= (seq_t)1 + (seq_t)tmp;   /* 1 + p */
        d_cnt  *= (seq_t)2;              /* exponent+1 = 2 */
    }

    return (sigma + d_cnt) / 2;       /* final value a(n) */
}
