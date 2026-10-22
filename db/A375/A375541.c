/*
 * A375541  a(n) = 2^n * n! * [x^n] (-1/2 - exp(-x))^n.
 * https://oeis.org/A375541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} k^n * 2^k * binomial(n,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t binom = 1;      /* C(n,0) */
    seq_t pow2 = 1;       /* 2^0 */

    for (idx_t k = 0; k <= n; ++k)
    {
        if (k > 0)
        {
            /* Update binomial coefficient C(n,k) */
            binom = (binom * (seq_t)(n - k + 1)) / (seq_t)k;
            /* Update 2^k */
            pow2 = pow2 * 2;
        }

        seq_t powk;
        if (k == 0)
        {
            /* 0^n is 1 only when n==0, otherwise 0 */
            powk = (n == 0) ? 1 : 0;
        }
        else
        {
            /* Compute k^n using the provided helper */
            powk = pow_uint(k, n);
        }

        /* term = C(n,k) * 2^k * k^n */
        sum += binom * powk * pow2;
    }

    return sum;
}
