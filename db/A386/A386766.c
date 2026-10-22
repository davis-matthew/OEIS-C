/*
 * A386766  a(n) = Sum_{k=0..n} 5^k * 2^(n-k) * binomial(n+k-1,k).
 * https://oeis.org/A386766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 5^k * 2^(n-k) * binomial(n+k-1,k)
 *            Recurrence: 9*n*a(n) +6*(-18*n-5)*a(n-1) +80*(-17*n+37)*a(n-2)
 *            +800*(-2*n+5)*a(n-3)=0 Initial values: a(0)=1, a(1)=7, a(2)=99
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)99;

    seq_t prev3 = (seq_t)1;   /* a(0) */
    seq_t prev2 = (seq_t)7;   /* a(1) */
    seq_t prev1 = (seq_t)99;  /* a(2) */
    seq_t an;

    for (idx_t i = 3; i <= n; ++i)
    {
        /* Use 128‑bit intermediate if seq_t is 64‑bit or smaller to avoid overflow */
        if (sizeof(seq_t) <= sizeof(unsigned long long))
        {
            __uint128_t coeff1 = ( __uint128_t )(18 * i + 5);
            __uint128_t coeff2 = ( __uint128_t )(17 * i - 37);
            __uint128_t coeff3 = ( __uint128_t )(2  * i - 5);

            __uint128_t num = 0;
            num += ( __uint128_t )6   * coeff1 * ( __uint128_t )prev1;
            num += ( __uint128_t )80  * coeff2 * ( __uint128_t )prev2;
            num += ( __uint128_t )800 * coeff3 * ( __uint128_t )prev3;

            __uint128_t denom = ( __uint128_t )(9 * i);
            an = (seq_t)(num / denom);
        }
        else
        {
            seq_t term1 = (seq_t)6 *
                          ((seq_t)(18 * i + 5)) * prev1;
            seq_t term2 = (seq_t)80 *
                          ((seq_t)(17 * i - 37)) * prev2;
            seq_t term3 = (seq_t)800 *
                          ((seq_t)(2 * i - 5)) * prev3;
            seq_t num = term1 + term2 + term3;
            an = num / ((seq_t)(9 * i));
        }

        /* shift previous values */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = an;
    }

    return prev1;
}
