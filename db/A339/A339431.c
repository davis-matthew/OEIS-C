/*
 * A339431  Number of compositions (ordered partitions) of n into an odd
 *          number of distinct squares.
 * https://oeis.org/A339431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 284, u32 n <= 818, u64 n <= 3310,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = ∑_{k odd} (number of subsets of distinct squares summing to n of size k) · k!
 *
 * Computation: dynamic programming over the set of squares ≤ n.
 * Count subsets of each size k that sum to each possible sum s.
 * The result is the sum of k! times the number of k‑sized subsets that sum to n,
 * taken only for odd k.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Determine the largest square ≤ n */
    idx_t max_k = 0;
    for (idx_t j = 1; j * j <= n; ++j)
        max_k = j;
    idx_t square_limit = max_k * max_k;

    /* Allocate DP table: dp[s][k] -> number of subsets of size k summing to s */
    size_t cap_k = (size_t)max_k + 1;                 /* columns for k */
    size_t cap_t = (size_t)n + 1;                    /* rows for sum */
    size_t total = cap_t * cap_k;
    seq_t *dp = (seq_t *)malloc(total * sizeof(seq_t));
    if (!dp) return (seq_t)0;                       /* out of memory safeguard */

    /* Initialize DP */
    for (size_t i = 0; i < total; ++i)
        dp[i] = 0;
    dp[0] = 1;   /* empty subset */

    /* DP over each square */
    idx_t processed = 0;
    for (idx_t s = 1; (idx_t)s * s <= n; ++s) {
        processed++;
        idx_t sq = (idx_t)s * s;
        /* Update DP backwards to avoid reuse of the same square */
        for (idx_t sum = n; sum >= sq; --sum) {
            size_t base = sum * cap_k;
            size_t prev_base = (sum - sq) * cap_k;
            for (idx_t k = processed; k >= 1; --k) {
                /* dp[sum][k] += dp[sum - sq][k - 1] */
                dp[base + k] += dp[prev_base + (k - 1)];
            }
        }
    }

    /* Precompute factorials up to max_k */
    seq_t *fact = (seq_t *)malloc(cap_k * sizeof(seq_t));
    if (!fact) {
        free(dp);
        return (seq_t)0;
    }
    fact[0] = 1;
    for (idx_t i = 1; i < cap_k; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* Sum over odd k */
    seq_t ans = 0;
    size_t base_n = n * cap_k;
    for (idx_t k = 1; k < cap_k; k += 2) {
        ans += dp[base_n + k] * fact[k];
    }

    free(dp);
    free(fact);
    return ans;
}
