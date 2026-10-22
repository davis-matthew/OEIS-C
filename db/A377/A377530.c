/*
 * A377530  Expansion of e.g.f. 1/(1 - x * exp(x))^3.
 * https://oeis.org/A377530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Sum_{k=0..n} k^(n-k) * binomial(k+2,2)/(n-k)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    seq_t fall   = 1; /* falling factorial P(n,0) */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t powk;
        if (k == 0)
        {
            powk = (n == 0) ? (seq_t)1 : (seq_t)0; /* 0^0 = 1, else 0 */
        }
        else
        {
            powk = big_pow((seq_t)k, (seq_t)(n - k));
        }

        /* binomial(k+2,2) = (k+2)*(k+1)/2 */
        seq_t comb = ((seq_t)(k + 2) * (seq_t)(k + 1)) / 2;

        seq_t term = fall;
        term *= powk;
        term *= comb;

        total += term;

        if (k < n)
        {
            /* update falling factorial: P(n, k+1) = P(n,k) * (n-k) */
            fall *= (seq_t)(n - k);
        }
    }

    return total;
}
