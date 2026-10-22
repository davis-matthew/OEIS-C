/*
 * A351017  Number of binary words of length n with all distinct run-lengths.
 * https://oeis.org/A351017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n)=2 * Σ_{k≥1} (number of subsets of distinct parts of
 *            size k that sum to n) * k!
 * Range:     u8 n <= 12, u16 n <= 29, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   _max_k_for_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_max_k_for_n.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Pre‑compute limits and allocate DP arrays. */
    idx_t kmax = _max_k_for_n(n);
    size_t cells = (size_t)(n + 1) * (kmax + 1);

    seq_t *dp = (seq_t *)malloc(cells * sizeof(seq_t));
    if (!dp) abort();
    for (size_t i = 0; i < cells; ++i) dp[i] = 0;
    dp[0] = 1;                                 /* sum 0 with 0 parts */

    /* 0/1 knapsack over distinct parts. */
    for (idx_t part = 1; part <= n; ++part) {
        for (idx_t i = n; i >= part; --i) {
            size_t baseCurr = (size_t)i * (kmax + 1);
            size_t basePrev = (size_t)(i - part) * (kmax + 1);
            for (idx_t k = 1; k <= kmax; ++k) {
                seq_t add = dp[basePrev + (k - 1)];
                if (add != 0)
                    dp[baseCurr + k] = dp[baseCurr + k] + add;
            }
        }
    }

    /* Factorials up to kmax. */
    seq_t *fact = (seq_t *)malloc((kmax + 1) * sizeof(seq_t));
    if (!fact) abort();
    fact[0] = 1;
    for (idx_t i = 1; i <= kmax; ++i) fact[i] = fact[i - 1] * (seq_t)i;

    /* Compute the required sum for n. */
    seq_t sum = 0;
    size_t start = (size_t)n * (kmax + 1);
    for (idx_t k = 1; k <= kmax; ++k) {
        sum = sum + dp[start + k] * fact[k];
    }
    seq_t ans = sum * 2;

    free(dp);
    free(fact);
    return ans;
}
