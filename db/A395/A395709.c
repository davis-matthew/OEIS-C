/*
 * A395709  Expansion of g^5*(5*g-4)/(4-3*g)^4, where g = 1+x*g^4 is the g.f.
 *          of A002293.
 * https://oeis.org/A395709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1) * Sum_{k=0..n} 4^(n-k) * binomial(3*n+k+3,k)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A395709. */

#include <stdint.h>
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* 4^n */
    seq_t pw = pow4(n);
    seq_t binom = 1;            /* binomial(3*n+3+0,0) = 1 */

    for (idx_t k = 0; k <= n; ++k)
    {
        sum += pw * binom;

        if (k == n)
            break;

        /* Update binomial for next k:
           binom(k+1) = binom(k) * (3*n + 4 + k) / (k+1)   */
        seq_t num = (seq_t)(3 * n + 4 + k);
        binom = (binom * num) / (k + 1);

        /* Update power of 4: pw(n-k-1) = pw(n-k)/4   */
        pw /= 4;
    }

    return (n + 1) * sum;
}
