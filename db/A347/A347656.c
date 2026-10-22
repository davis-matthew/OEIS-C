/*
 * A347656  Number of partitions of n into at most 5 squarefree parts.
 * https://oeis.org/A347656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into at most 5 squarefree parts
 * Range:     u8 n <= 33, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A347656) */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* cast to size_t for array indexing */
    size_t N = (size_t)n;
    /* Allocate DP table: 6 rows (0..5 parts) each with N+1 columns */
    seq_t *dp[6];
    for (int p = 0; p < 6; ++p) {
        dp[p] = (seq_t*)calloc(N + 1, sizeof(seq_t));
        if (!dp[p]) return (seq_t)0; /* out of memory */
    }
    dp[0][0] = 1; /* one way to make sum 0 with 0 parts */

    /* Determine squarefree numbers up to N using sieve of squares */
    seq_t *sqfree = (seq_t*)calloc(N + 1, sizeof(seq_t));
    if (!sqfree) { /* free dp before returning */
        for (int p = 0; p < 6; ++p) free(dp[p]);
        return (seq_t)0;
    }
    for (size_t i = 1; i <= N; ++i) sqfree[i] = 1;            /* assume true */
    for (size_t p = 2; p * p <= N; ++p) {                     /* mark non‑squarefree */
        size_t sq = p * p;
        for (size_t j = sq; j <= N; j += sq) sqfree[j] = 0;
    }

    /* DP over each squarefree coin */
    for (size_t v = 1; v <= N; ++v) {
        if (!sqfree[v]) continue;
        for (size_t s = v; s <= N; ++s) {
            for (int part = 1; part <= 5; ++part) {
                dp[part][s] += dp[part - 1][s - v];
            }
        }
    }

    /* Sum contributions from 0 to 5 parts for sum N */
    seq_t ans = 0;
    for (int part = 0; part <= 5; ++part) ans += dp[part][N];

    /* Free memory */
    for (int p = 0; p < 6; ++p) free(dp[p]);
    free(sqfree);

    return ans;
}
