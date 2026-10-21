/*
 * A241247  a(n) = Sum_{k=0..n} n^k * binomial(n,k)^3.
 * https://oeis.org/A241247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} n^k * binomial(n,k)^3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"   /* Provides binomial(idx_t,idx_t) */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t pk   = 1;            /* pk = n^k, starts with k=0 */
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t b   = binomial(n, k);          /* binomial(n,k) */
        seq_t b2  = b * b;                   /* b^2   */
        seq_t b3  = b2 * b;                  /* b^3   */
        seq_t term = pk * b3;                /* n^k * b^3 */
        sum += term;
        pk   *= (seq_t)n;                    /* advance to n^(k+1) */
    }
    return sum;
}
