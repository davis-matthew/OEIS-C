/*
 * A393991  Expansion of g.f.: Product_{k>=1} (1 + x^(k^2 + 1)).
 * https://oeis.org/A393991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts of the form
 *            k^2+1 (k>=1)
 * Range:     u8 n <= 88, u16 n <= 2367, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;   /* dp[j] holds a(j) for j ≤ cur_max */
static idx_t cur_max = 0;  /* largest n currently computed */

/* Computes a(n) for any n ≥ 0.  The function is thread‑unsafe but
   satisfies the requirements for memoized, incremental computation. */
seq_t a(idx_t n)
{
    /* Initialise for the first call */
    if (dp == NULL) {
        dp = (seq_t *)calloc(1, sizeof(seq_t));
        if (!dp) return 0;          /* allocation failure fallback   */
        dp[0] = 1;
        cur_max = 0;
    }

    if (n <= cur_max)
        return dp[n];

    /* Recompute the DP up to the new maximum n */
    idx_t new_max = n;
    seq_t *tmp = (seq_t *)realloc(dp, (new_max + 1) * sizeof(seq_t));
    if (!tmp) return 0;              /* allocation failure fallback   */
    dp = tmp;

    /* Reset all entries (except dp[0]) to 0 before recomputation */
    for (idx_t i = 1; i <= new_max; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* 0‑1 knapsack over the distinct parts k^2+1 */
    for (idx_t k = 1; ; ++k) {
        seq_t v_seq = (seq_t)k * (seq_t)k + 1;   /* k^2 + 1 */
        if (v_seq > new_max)
            break;
        idx_t v = (idx_t)v_seq;
        for (idx_t j = new_max; j >= v; --j)
            dp[j] += dp[j - v];
    }

    cur_max = new_max;
    return dp[n];
}
