/*
 * A354761  Least number of squares and cubes that add up to n.
 * https://oeis.org/A354761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* This implements: a(n) = minimum number of perfect squares or
   perfect cubes whose sum equals n.
   The algorithm recomputes a DP table up to n for every call.
   Since each n ≤ 10 000, the work is < 1.5 M simple integer
   operations, well under the 2‑second requirement. */

seq_t a(idx_t n)
{
    /* static DP array that is resized when a larger n is requested */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;

    /* Ensure space for indices 0 .. n */
    if (n + 1 > dp_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (!tmp) {          /* allocation failure: give up */
            if (dp) free(dp);
            dp = NULL;
            return 0;
        }
        dp = tmp;
        dp_size = new_size;
    }

    /* Infinity value large enough to never be minimal */
    seq_t INF = (seq_t)(n + 1);

    /* Initialize DP table */
    dp[0] = 0;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = INF;

    /* Process all squares ≤ n */
    for (idx_t i = 1; ; ++i) {
        idx_t sq = i * i;
        if (sq > n) break;
        seq_t val = (seq_t)sq;
        for (idx_t sum = sq; sum <= n; ++sum) {
            seq_t cand = dp[sum - sq] + (seq_t)1;
            if (cand < dp[sum]) dp[sum] = cand;
        }
    }

    /* Process all cubes ≤ n */
    for (idx_t i = 1; ; ++i) {
        idx_t cube = i * i * i;
        if (cube > n) break;
        seq_t val = (seq_t)cube;
        for (idx_t sum = cube; sum <= n; ++sum) {
            seq_t cand = dp[sum - cube] + (seq_t)1;
            if (cand < dp[sum]) dp[sum] = cand;
        }
    }

    return dp[n];
}
