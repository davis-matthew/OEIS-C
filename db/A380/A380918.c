/*
 * A380918  Expansion of e.g.f. (1/x) * Series_Reversion( x * (1 - x) *
 *          exp(-2*x/(1 - x)) ).
 * https://oeis.org/A380918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} 2^k * (n+1)^(k-1) * binomial(2*n, n-k)
 *            / k!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Derived from the e.g.f. definition in OEIS A380918.) */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

/* Compute a(n) for any non‑negative n.
   Uses only integer arithmetic on seq_t, which may be a built‑in
   unsigned type (up to 128 bits) or an arbitrary‑precision integer class. */
seq_t a(idx_t n)
{
    /* factorial(n) */
    seq_t fac_n = 1;
    for (idx_t i = 1; i <= n; ++i)
        fac_n *= (seq_t)i;

    /* initial binomial(2n, n) */
    seq_t binom_cur = binomial(2 * n, n);

    /* term for k = 0: binom(2n,n)/(n+1) * n! */
    seq_t sum = (binom_cur / (seq_t)(n + 1)) * fac_n;

    /* Variables for incremental computation */
    seq_t ratio   = fac_n;          /* fac[n] / fac[k]   (k=0)   */
    seq_t pow2    = 1;              /* 2^(k-1)           (k=1)  */
    seq_t pow_n   = 1;              /* (n+1)^(k-1)       (k=1)  */

    for (idx_t k = 1; k <= n; ++k) {
        /* update 2^k */
        pow2 *= (seq_t)2;

        /* update ratio = fac[n]/fac[k] */
        ratio /= (seq_t)k;

        /* update binomial(2n, n-k) */
        binom_cur = (binom_cur * (seq_t)(n - k + 1)) / (seq_t)(n + k);

        /* term for this k */
        seq_t term = binom_cur * ratio * pow2 * pow_n;
        sum += term;

        /* prepare pow_n for next iteration: (n+1)^(k) */
        pow_n *= (seq_t)(n + 1);
    }

    return sum;
}
