/*
 * A383139  a(n) = Sum_{k=0..n} k^5 * 2^(n-k) * binomial(n,k).
 * https://oeis.org/A383139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} k^5 * 2^(n-k) * binomial(n,k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 29, u128 n <= 66,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t total   = 0;          /* cumulative sum */
    seq_t c       = 1;          /* binomial coefficient C(n,0) */
    seq_t pow2_val = pow2(n);   /* 2^(n-k) for k = 0 */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* compute k^5 as a product of powers of k */
        seq_t kk  = (seq_t)k;
        seq_t k2  = kk * kk;        /* k^2 */
        seq_t k3  = k2 * kk;        /* k^3 */
        seq_t k4  = k3 * kk;        /* k^4 */
        seq_t k5  = k4 * kk;        /* k^5 */

        /* term = k^5 * C(n,k) * 2^(n-k) */
        seq_t term = k5 * c;
        term = term * pow2_val;

        total += term;

        /* update binomial coefficient and power of two for next k */
        if (k < n)
        {
            c = c * (seq_t)(n - k);   /* multiply by (n-k) */
            c = c / (seq_t)(k + 1);   /* divide by (k+1) */
            pow2_val >>= 1;           /* divide by 2 */
        }
    }

    return total;
}
