/*
 * A329439  Expansion of Sum_{k>=1} (-1 + Product_{j>=1} 1 / (1 - x^(k*j^2))).
 * https://oeis.org/A329439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A001156(d) (A001156 = partitions into squares)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache of partition‑into‑squares counts dp[0..dp_size] */
static seq_t *dp = NULL;   /* dp[0] = 1, dp[i] = p_squares(i) */
static idx_t dp_size = 0;

/* Recompute dp up to N (inclusive).  All values are assumed to fit in seq_t. */
static void recompute_dp(idx_t N) {
    if (dp) free(dp);                         /* discard old cache if any */

    /* Allocate zeroed array of length N+1 */
    dp = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    if (!dp) exit(1);                          /* allocation failure */

    dp[0] = 1;                                 /* empty partition */

    /* Generate all square numbers ≤ N and apply the classic integer partition DP. */
    for (idx_t j = 1; j * j <= N; ++j) {
        idx_t sq = j * j;
        for (idx_t s = sq; s <= N; ++s) {
            dp[s] += dp[s - sq];
        }
    }
    dp_size = N;
}

/* Public routine */
seq_t a(idx_t n) {
    if (n == 0) return 0;                          /* safety, although n ≥ 1 in the spec */

    /* Ensure dp contains values up to n */
    if (dp_size < n) recompute_dp(n);

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += dp[d];
            idx_t e = n / d;
            if (e != d) sum += dp[e];
        }
    }
    return sum;
}
