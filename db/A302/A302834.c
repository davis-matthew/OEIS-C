/*
 * A302834  Expansion of (1/(1 - x))*Product_{k>=1} 1/(1 - x^(k^3)).
 * https://oeis.org/A302834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=0}^n p_cubes(i) where p_cubes(m) is the number of
 *            partitions of m into cubes (including 1^3). The generating
 *            function is 1/(1-x) · ∏_{k≥1} 1/(1-x^{k³})
 * Range:     u8 n <= 51, u16 n <= 316, u32 n <= 2540, u64 n <= 9204,
 *            u128 n <= 9204, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* static tables holding partitions into cubes (dp_)
   and their cumulative sums (cum_) */
static seq_t *dp_   = NULL;   /* dp_[i] = number of partitions of i into cubes */
static seq_t *cum_  = NULL;   /* cum_[i] = a(i) = Σ_{j=0}^i dp_[j] */
static idx_t  avail_limit = 0; /* largest n that has been computed */

/* a small pre‑computed limit that covers all normal test inputs */
static const idx_t PRECOMP_LIMIT = 20000; /* recompute up to this size once */

/* recompute tables for all n ≤ limit */
static void recompute(idx_t limit)
{
    seq_t *new_dp, *new_cum;

    new_dp  = (seq_t*)malloc((size_t)(limit + 1) * sizeof(seq_t));
    new_cum = (seq_t*)malloc((size_t)(limit + 1) * sizeof(seq_t));
    if (!new_dp || !new_cum)
        abort();  /* allocation failure */

    /* partitions into cubes */
    new_dp[0] = 1;                     /* one way to partition 0 */
    for (idx_t i = 1; i <= limit; ++i)
        new_dp[i] = 0;                  /* initialise rest to zero */

    /* classic coin–change DP: each cube is a coin of value k³ */
    for (idx_t t = 1; ; ++t) {
        idx_t cube = t * t * t;        /* k³ */
        if (cube > limit)
            break;
        for (idx_t i = cube; i <= limit; ++i)
            new_dp[i] += new_dp[i - cube];
    }

    /* cumulative sums give a(n) */
    new_cum[0] = new_dp[0];
    for (idx_t i = 1; i <= limit; ++i)
        new_cum[i] = new_cum[i - 1] + new_dp[i];

    /* replace old tables */
    free(dp_);
    free(cum_);
    dp_  = new_dp;
    cum_ = new_cum;

    avail_limit = limit;
}

/* exported function required by the test harness */
seq_t a(idx_t n)
{
    /* Initialise tables once (pre‑computing up to PRECOMP_LIMIT). */
    if (!dp_ || !cum_) {
        if (PRECOMP_LIMIT == 0) {
            /* Fallback (does not occur with the chosen limit). */
            recompute(n);
        } else {
            recompute(PRECOMP_LIMIT);
        }
    }

    /* Extend tables if the requested n exceeds the current limit. */
    if (n > avail_limit)
        recompute(n);

    return cum_[n];
}
