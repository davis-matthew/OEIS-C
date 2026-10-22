/*
 * A309266  Expansion of (1 + x) * Product_{k>=1} (1 + x^k)/(1 - x^k).
 * https://oeis.org/A309266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of ((1+x) * ∏_{k≥1} (1+x^k)/(1−x^k)) at x^n
 *            Computed by iteratively multiplying the generating function
 *            factors. The recurrence is dp_0[0] = 1, dp_0[i>0] = 0 dp_{k}[n]
 *            = dp_{k-1}[n] + 2 * Σ_{m≥1, n−m·k ≥ 0} dp_{k-1}[n−m·k] (for k ≥
 *            1) Final answer a(n) = dp_n[n] + dp_n[n−1] (wi...
 * Range:     u8 n <= 9, u16 n <= 25, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static memoisation state ------------------------------------------ */
static seq_t *dp      = NULL;   /* dp array for the current product      */
static seq_t *tmp     = NULL;   /* temporary array for intermediate step */
static size_t  cap    = 0;      /* allocated size of dp and tmp          */

/* ensure array capacity for indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    idx_t needed = n + 1;          /* indices 0..n */
    if (cap < needed)
    {
        dp  = (seq_t *)realloc(dp,  needed * sizeof(seq_t));
        tmp = (seq_t *)realloc(tmp, needed * sizeof(seq_t));
        for (idx_t i = cap; i < needed; ++i)     /* initialise new cells */
            dp[i] = tmp[i] = (seq_t)0;
        cap = needed;
    }
}

/* --------------------------------------------------------------------
   Compute a(n) for any n ≥ 0.  The computation is performed from scratch
   each time; this satisfies requirement 6 (any order of calls).  The loops
   are straightforward integer operations and should run well under the
   time limit for n up to about 10000. */
seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0)
        return (seq_t)1;

    /* ensure we have storage for indices 0..n */
    ensure_capacity(n);

    /* initialise dp[0] = 1, all other dp[i] = 0 */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* iterate over part sizes k = 1 .. n */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* zero temporary array for this factor */
        for (idx_t i = 0; i <= n; ++i)
            tmp[i] = (seq_t)0;

        /* apply factor (1 + 2x^k + 2x^{2k} + …) */
        for (idx_t i = 0; i <= n; ++i)
        {
            seq_t val = dp[i];
            if (val == 0)        /* skip excessive work for zero terms */
                continue;

            /* contribution for m = 0 */
            tmp[i] += val;

            /* contributions for m ≥ 1 with coefficient 2 */
            for (idx_t j = i + k; j <= n; j += k)
                tmp[j] += (seq_t)2 * val;
        }

        /* copy tmp back into dp for the next iteration */
        for (idx_t i = 0; i <= n; ++i)
            dp[i] = tmp[i];
    }

    /* a(n) = dp[n] + dp[n-1] (dp[-1] is implicitly zero) */
    return dp[n] + dp[n - 1];
}
