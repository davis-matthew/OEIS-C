/*
 * A294642  a(n) = n! * [x^n] exp(n*x)*BesselI(1,2*sqrt(2)*x)/(sqrt(2)*x).
 * https://oeis.org/A294642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} 2^k * n^(n-2k) * binomial(n,2k) *
 *            Catalan(k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   pow2, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the closed formula for OEIS A294642. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_pow.h"   /* seq_pow(base, exp) → seq_t */
#include "../utils/pow2.h"      /* pow2(k) → seq_t, but we use an incremental loop */

seq_t a(idx_t n)
{
    /* Handle the trivial cases first */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* Helper constants */
    idx_t maxk = n / 2;                     /* floor(n/2) */
    seq_t n_sq = (seq_t)n * (seq_t)n;       /* n^2, used for dividing pow_n */

    /* Start values for the accumulators */
    seq_t ans      = (seq_t)0;             /* sum of terms */
    seq_t pow_n    = seq_pow((seq_t)n, n); /* n^n   (will be divided by n^2 each loop) */
    seq_t catalan  = (seq_t)1;             /* Catalan(0) */
    seq_t pow2_val = (seq_t)1;             /* 2^k   (kept incrementally) */
    seq_t bin      = (seq_t)1;             /* binomial(n, 0) */

    for (idx_t k = 0; k <= maxk; ++k) {
        /* term = pow_n * bin * catalan * pow2_val */
        seq_t term = pow_n * bin;
        term *= catalan;
        term *= pow2_val;
        ans += term;

        if (k == maxk) break;              /* finished the last term */

        /* Update pow_n for the next k (divide by n^2) */
        if (n_sq != 0)
            pow_n = pow_n / n_sq;          /* exact division: n^(n-2k-2) */

        /* Update Catalan(k) → Catalan(k+1) */
        /* Catalan(k+1) = Catalan(k) * 2*(2k+1) / (k+2) */
        seq_t mult   = (seq_t)2 * (seq_t)(2 * k + 1);
        catalan = (catalan * mult) / ((seq_t)(k + 2));

        /* Update 2^k → 2^(k+1) */
        pow2_val = pow2_val * 2;

        /* Update binomial for the next even index 2*(k+1) */
        idx_t m = 2 * k;                   /* current even index below 2k <= n */
        /* Compute binom(n, m+1) from binom(n, m) */
        seq_t tmp = bin * ((seq_t)(n - m));
        bin = tmp / ((seq_t)(m + 1));
        /* Compute binom(n, m+2) from binom(n, m+1) */
        tmp = bin * ((seq_t)(n - (m + 1)));
        bin = tmp / ((seq_t)(m + 2));
    }

    return ans;
}
