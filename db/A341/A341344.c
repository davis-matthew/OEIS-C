/*
 * A341344  a(n) = A100631(n, floor(n/2)).
 * https://oeis.org/A341344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A100631(n, floor(n/2)) Computed via terminating
 *            hypergeometric sum: a(n) = 2^{ceiling(n/2)} *
 *            Σ_{k=0}^{floor(n/2)-1} binom(floor(n/2)-1, k) *
 *            binom(ceiling(n/2)-1 + k, k)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    idx_t m = n / 2;           /* floor(n/2) */
    idx_t p = n - m;           /* ceil(n/2) */

    /* Handle n = 0, 1 directly. */
    if (m == 0)
        return (seq_t)1;

    seq_t sum  = 0;
    seq_t curA = 1;            /* binom(m-1, k)   */
    seq_t curB = 1;            /* binom(p-1+k, k) */

    for (idx_t k = 0; k < m; ++k)
    {
        if (k > 0)
        {
            /* Update binom(m-1, k) using the multiplicative formula. */
            curA = curA * (seq_t)(m - k) / (seq_t)k;

            /* Update binom(p-1+k, k) similarly. */
            curB = curB * (seq_t)(p - 1 + k) / (seq_t)k;
        }
        sum += curA * curB;
    }

    seq_t pow2 = seq_pow((seq_t)2, p);
    return sum * pow2;
}
