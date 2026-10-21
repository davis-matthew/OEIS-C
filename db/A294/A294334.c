/*
 * A294334  Number of partitions of n into triangular numbers dividing n.
 * https://oeis.org/A294334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of partitions of n into triangular numbers
 *            dividing n
 * Range:     u8 n <= 59, u16 n <= 209, u32 n <= 1259, u64 n <= 7559,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* list of triangular divisors of n */
    seq_t coins[200]; /* enough for n <= 10^9, but our constraints are small */
    idx_t coin_cnt = 0;
    idx_t r = 1;
    while (1) {
        seq_t tri = (seq_t)r * ((seq_t)r + 1) / 2;
        if (tri > (seq_t)n) break;
        if (n % (idx_t)tri == 0) {
            coins[coin_cnt++] = tri;
        }
        ++r;
    }

    /* dp[i] = number of partitions of i with the allowed coins */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0; /* allocation failure – unlikely in the given scale */

    /* initialize */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    /* unbounded coin change DP (order of coins guarantees combinations) */
    for (idx_t ci = 0; ci < coin_cnt; ++ci) {
        idx_t coin = (idx_t)coins[ci];
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] += dp[i - coin];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
