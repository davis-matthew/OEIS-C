/*
 * A308911  Sum of the largest parts in the partitions of n into 6 squarefree
 *          parts.
 * https://oeis.org/A308911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum of the largest parts in the partitions of n into 6
 *            squarefree parts. Implementation: dynamic programming over all
 *            squarefree numbers ≤ n‑5. For each squarefree number m (chosen
 *            as the largest part), we add m * P(n‑m,5), where P(s, k) is the
 *            number of partitions of s into exactly k squa...
 * Range:     u8 n <= 19, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 6)            /* no partitions into 6 parts */
        return (seq_t)0;

    idx_t N = n;
    idx_t max_part = n - 5;          /* largest part can never exceed n‑5 */

    /* ---------- compute list of squarefree numbers up to max_part ---------- */
    uint8_t *sqfree = (uint8_t*)malloc((max_part + 1) * sizeof(uint8_t));
    if (!sqfree) return (seq_t)0;
    for (idx_t i = 0; i <= max_part; ++i) sqfree[i] = 1;   /* initialize as squarefree */

    for (idx_t p = 2; (idx_t)p * p <= max_part; ++p) {
        idx_t sq = (idx_t)p * p;
        for (idx_t j = sq; j <= max_part; j += sq)
            sqfree[j] = 0;
    }

    /* gather squarefree numbers into a list */
    idx_t *sf = (idx_t*)malloc((max_part + 1) * sizeof(idx_t));
    if (!sf) { free(sqfree); return (seq_t)0; }
    idx_t sf_cnt = 0;
    for (idx_t i = 1; i <= max_part; ++i)
        if (sqfree[i]) sf[sf_cnt++] = i;
    free(sqfree);      /* no longer needed */

    /* ---------- DP table: dp[sum*6 + k]  (k = 0..5) ---------- */
    size_t dp_size = (size_t)(N + 1) * 6;
    seq_t *dp = (seq_t*)calloc(dp_size, sizeof(seq_t));
    if (!dp) { free(sf); return (seq_t)0; }
    dp[0] = 1;          /* dp[0][0] = 1 */

    seq_t result = 0;

    /* ---------- iterate over squarefree numbers as candidate largest part ---------- */
    for (idx_t idx = 0; idx < sf_cnt; ++idx) {
        idx_t x = sf[idx];

        /* update DP to include this part with unlimited multiples */
        for (idx_t s = x; s <= N; ++s) {
            size_t base_s   = s   * 6;
            size_t base_xy  = (s - x) * 6;
            for (idx_t k = 1; k <= 5; ++k)
                dp[base_s + k] += dp[base_xy + (k - 1)];
        }

        /* contribution of partitions whose largest part is x */
        idx_t rem = N - x;
        seq_t ways = dp[rem * 6 + 5];
        result += (seq_t)x * ways;
    }

    free(sf);
    free(dp);
    return result;
}
