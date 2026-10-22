/*
 * A379860  Expansion of e.g.f. (1/x) * Series_Reversion( x * exp(-x) * (1 -
 *          x*exp(x))^2 ).
 * https://oeis.org/A379860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!/(n+1)) * Σ_{k=0..n} (n+k+1)^(n-k) * binomial(2*n+k+1,
 *            k) / (n-k)!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

#if defined(__SIZEOF_INT128__) && __SIZEOF_INT128__ == 16
    /* Use 128‑bit intermediate arithmetic when seq_t is an integral type */
    typedef unsigned __int128 big_t;
#else
    /* Fallback: use seq_t itself for intermediate storage */
    typedef seq_t big_t;
#endif

seq_t a(idx_t n)
{
    big_t sum = 0;
    seq_t mk     = 1;   /* n!/(n-k)! for current k */
    seq_t binom  = 1;   /* binomial(2*n+k+1, k) for current k */

    for (idx_t k = 0; k <= n; ++k) {
        /* (n+k+1)^(n-k) */
        seq_t base = (seq_t)n + k + 1;
        seq_t pow_term = big_pow(base, (idx_t)(n - k));

        /* term_k = [n!/(n-k)!] * pow_term * binom_k */
        big_t term = (big_t)mk * (big_t)pow_term * (big_t)binom;
        sum += term;

        if (k < n) {
            /* Update mk : mk *= (n - k)  → n!/(n-(k+1))!   */
            mk *= (seq_t)(n - k);

            /* Update binom : binom_{k+1} = binom_k * (2*n + k + 2)/(k+1) */
            seq_t numerator   = (seq_t)(2 * n + k + 2);
            seq_t denominator = (seq_t)(k + 1);
            binom = (binom * numerator) / denominator;
        }
    }

    /* a(n) = sum / (n+1) */
    return (seq_t)(sum / (big_t)((seq_t)n + 1));
}
