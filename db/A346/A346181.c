/*
 * A346181  a(n) = Sum_{k=0..n} binomial(n,k) * k^n * (k+1)^(n-1), with
 *          a(0)=1.
 * https://oeis.org/A346181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * k^n * (k+1)^(n-1) (a(0)=1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)0;
    seq_t bin    = (seq_t)1;              /* binomial(n,0) */
    for (idx_t k = 0; k <= n; ++k)
    {
        /* k^n and (k+1)^(n-1) */
        seq_t k_pow      = big_pow((seq_t)k,     (seq_t)n);
        seq_t kp1_pow    = big_pow((seq_t)(k+1), (seq_t)(n-1));

        /* term = bin * k^n * (k+1)^(n-1) */
        seq_t term = bin;
        term = term * k_pow;
        term = term * kp1_pow;

        result += term;

        /* update binomial to binomial(n,k+1) for next iteration */
        if (k < n)
        {
            seq_t tmp = bin * (seq_t)(n - k);
            bin = tmp / (seq_t)(k + 1);
        }
    }
    return result;
}
