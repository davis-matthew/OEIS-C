/*
 * A261737  Number of partitions of n where each part i is marked with a word
 *          of length i over a ternary alphabet whose letters appear in
 *          alphabetical order.
 * https://oeis.org/A261737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of ∏_{k≥1} 1/(1 - binomial(k+2,2)·x^k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    size_t N = (size_t)n;

    /* static cache for dynamic‑programming array */
    static seq_t *dp = NULL;
    static size_t dp_len = 0;

    /* ensure the array is large enough */
    if (dp_len < N + 1) {
        seq_t *new_dp = (seq_t*)malloc((N + 1) * sizeof(seq_t));
        if (!new_dp) return 0;   /* allocation failure */
        free(dp);
        dp = new_dp;
        dp_len = N + 1;
    }

    /* initialise up to N */
    for (size_t i = 0; i <= N; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* DP: dp[j] += dp[j-i] * C(i),   C(i)=binom(i+2,2) */
    for (size_t i = 1; i <= N; ++i) {
        seq_t ci = ((seq_t)(i + 2) * (seq_t)(i + 1)) / 2;
        for (size_t j = i; j <= N; ++j) {
            dp[j] += dp[j - i] * ci;
        }
    }

    return dp[N];
}
