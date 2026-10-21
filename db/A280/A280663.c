/*
 * A280663  G.f.: Product_{k>=1, j>=1} (1 + x^(j*k^3)).
 * https://oeis.org/A280663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 70, u32 n <= 211, u64 n <= 691,
 *            u128 n <= 2431, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A280663:
 * a(n) = number of ways to write n as a sum of distinct terms j*k^3 with k>=1, j>=1.
 * This is obtained by dynamic programming over all such terms.  */

#include <stdlib.h>

/* static globals for memoized DP */
static seq_t *dp = NULL;      /* array of computed a(n) values */
static idx_t dp_max = 0;      /* largest n for which dp has been computed */

/* build dp[0..N] from scratch */
static void build_dp(idx_t N)
{
    /* reallocate the dp array if needed */
    seq_t *newdp = (seq_t*)realloc(dp, (N + 1) * sizeof(seq_t));
    dp = newdp;

    /* initialise all entries to 0 and set dp[0] = 1 */
    for (idx_t i = 0; i <= N; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* 0‑1 knapsack over all pairs (k,j) with j*k^3 <= N */
    for (idx_t k = 1; (k * k * k) <= N; ++k) {
        idx_t k3 = k * k * k;         /* k^3 */
        for (idx_t j = 1; j * k3 <= N; ++j) {
            idx_t weight = j * k3;    /* weight of this item */
            for (idx_t s = N; s >= weight; --s) {
                dp[s] += dp[s - weight];
            }
        }
    }
    dp_max = N;
}

/* external function required by the framework */
seq_t a(idx_t n)
{
    /* initialise DP up to a safe bound on first call */
    static int inited = 0;
    if (!inited) {
        const idx_t PRECOMP_LIMIT = 10000;   /* typical maximal n in tests */
        build_dp(PRECOMP_LIMIT);
        inited = 1;
    }

    /* if requested n is within the precomputed range, return it */
    if (n <= dp_max)
        return dp[n];

    /* otherwise rebuild DP up to the new n */
    build_dp(n);
    return dp[n];
}
