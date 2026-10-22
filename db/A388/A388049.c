/*
 * A388049  a(n) = Sum_{k=0..n} 3^k * binomial(n,k) * binomial(3*n+1,k).
 * https://oeis.org/A388049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3^k * binomial(n,k) * binomial(3*n+1,k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1 */

#include "../utils/binomial.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum   = 0;
    seq_t pow3k = 1;                 /* 3^k for current k          */
    idx_t n3    = 3 * n + 1;          /* 3*n+1 for binomial         */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t bin1 = binomial(n,   k);   /* binomial(n,k)        */
        seq_t bin2 = binomial(n3, k);    /* binomial(3*n+1,k)    */

        seq_t term = bin1 * bin2;        /* product of binomials */
        term *= pow3k;                   /* multiply by 3^k       */

        sum += term;                     /* accumulate sum        */

        pow3k *= 3;                      /* update 3^k for next iteration */
    }
    return sum;
}
