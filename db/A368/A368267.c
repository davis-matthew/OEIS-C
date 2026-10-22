/*
 * A368267  Expansion of e.g.f. exp(2*x) / (1 - 2*x*exp(x)).
 * https://oeis.org/A368267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n!*Sum_{k=0..n} 2^(n-k)*(n-k+2)^k/k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* compute n! */
    seq_t fact_n = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        fact_n *= (seq_t)i;

    /* compute 2^n */
    seq_t pow2_n = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        pow2_n *= (seq_t)2;

    seq_t pref   = fact_n * pow2_n;   /* initially n! * 2^n   */
    seq_t binom  = (seq_t)1;          /* C(n,0) = 1           */
    seq_t sum    = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k)
    {
        idx_t base = n - k + 2;
        seq_t pow_base = pow_int((seq_t)base, k);

        sum += pref * binom * pow_base;

        if (k < n)               /* update for next k */
        {
            pref  /= ((idx_t)(n - k) * (idx_t)2);    /* A_{k+1} = A_k / ((n-k)*2) */
            binom  = binom * (idx_t)(n - k) / (k + 1); /* C(n,k+1) = C(n,k)*(n-k)/(k+1) */
        }
    }

    return sum;
}
