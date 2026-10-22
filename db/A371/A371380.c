/*
 * A371380  Expansion of (1/x) * Series_Reversion( x * (1-3*x)^2 / (1-x) ).
 * https://oeis.org/A371380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..n} 2^k * binomial(2*n+k+1,k) *
 *            binomial(2*n,n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Use 128-bit arithmetic for intermediate values to avoid overflow
       when seq_t is 64‑bit.  For arbitrary‑precision builds the casts
       to __uint128_t are no‑ops because seq_t can hold all values. */
    if (n == 0) return (seq_t)1;   /* trivial case */

    __uint128_t sum  = 0;
    __uint128_t pow2 = 1;                     /* 2^k  */
    __uint128_t bin1 = 1;                     /* C(2*n+k+1,k)  */
    idx_t twon      = n << 1;                 /* 2*n             */
    __uint128_t bin2 = (__uint128_t)binomial(twon, n); /* C(2*n,n)        */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term = 2^k * C(2n+k+1,k) * C(2n,n-k) */
        __uint128_t term = pow2 * bin1 * bin2;
        sum += term;

        if (k == n) break;   /* last iteration – avoid over‑division */

        /* Update for next k */
        pow2 = pow2 * 2;
        bin1 = bin1 * (twon + k + 2) / (k + 1);               /* C(2n+k+2,k+1)   */
        bin2 = bin2 * (n - k) / (n + k + 1);                  /* C(2n,n-k-1)     */
    }

    /* Final division by n+1 */
    return (seq_t)(sum / (__uint128_t)(n + 1));
}
