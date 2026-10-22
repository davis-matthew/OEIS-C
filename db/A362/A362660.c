/*
 * A362660  E.g.f. satisfies A(x) = exp( x * exp(x^2/2) * A(x) ).
 * https://oeis.org/A362660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..⌊n/2⌋} (n-2k)^k * (n-2k+1)^(n-2k-1) / (2^k *
 *            k! * (n-2k)!) (OEIS A362660)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t   maxk = n / 2;
    seq_t   sum = (seq_t)0;
    seq_t   f   = (seq_t)1;   /* f_k = n! / ( (n-2k)! * k! * 2^k ) */

    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t m = n - 2 * k;          /* n-2k, >0 unless k = n/2 */
        if (m == 0)                   /* term would be 0 for k > 0 */
            break;

        seq_t pow1 = big_pow((seq_t)m,      (seq_t)k);      /* (n-2k)^k   */
        seq_t pow2 = big_pow((seq_t)(m + 1), (seq_t)(m - 1));/* (n-2k+1)^(n-2k-1) */

        seq_t term = f;
        term *= pow1;
        term *= pow2;
        sum += term;

        if (k == maxk)                /* last needed term */
            break;

        /* Update f_{k+1} = f_k * m * (m-1) / ((k+1)*2) */
        seq_t num = f;
        num *= (seq_t)m;
        num *= (seq_t)(m - 1);

        seq_t denom = (seq_t)((k + 1) * 2);
        f = num / denom;
    }

    return sum;
}
