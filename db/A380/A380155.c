/*
 * A380155  Expansion of e.g.f. 1/sqrt(1 - 2*x*exp(2*x)).
 * https://oeis.org/A380155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * n! * Sum_{k=0..n} (-1)^k * k^(n-k) *
 *            binomial(-1/2,k)/(n-k)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Equivalent algebraic form: a(n) = Σ_{k=0..n} binom(n,k) * 2^(n-k) * k^(n-k) * (2k-1)!! ) */

#include <stddef.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* 2^n */
    seq_t pow2_cur = seq_pow((seq_t)2, n);

    /* binomial coefficient binom(n,0) = 1 */
    seq_t binom = 1;

    /* double factorial (2k-1)!! for current k, start with k = 0 */
    seq_t double_fact_cur = 1;

    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k) {
        /* k^(n-k) */
        seq_t kpow = seq_pow((seq_t)k, n - k);

        /* term = binom * pow2_cur * kpow * double_fact_cur */
        seq_t term = binom;
        term *= pow2_cur;
        term *= kpow;
        term *= double_fact_cur;
        sum += term;

        if (k == n)
            break; /* no need to update for next iteration */

        /* Update binomial coefficient for next k: binom(n,k+1) = binom * (n-k)/(k+1) */
        seq_t num   = (seq_t)(n - k);
        seq_t denom = (seq_t)(k + 1);
        binom = (binom * num) / denom;

        /* Update 2^(n-(k+1)) from current 2^(n-k) */
        pow2_cur = pow2_cur / (seq_t)2;

        /* Update double factorial to (2(k+1)-1)!! */
        double_fact_cur *= (seq_t)(2 * (k + 1) - 1);
    }

    return sum;
}
