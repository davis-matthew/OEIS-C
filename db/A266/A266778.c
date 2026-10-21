/*
 * A266778  Molien series for invariants of finite Coxeter group A_9.
 * https://oeis.org/A266778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = Σ_{k=2}^{10} a(n‑k) for n ≥ 1 (a(n) = 0 for n <
 *            0). This counts partitions of n into parts 2–10 inclusive; the
 *            Molien series generating function is 1/∏_{i=2}^{10} (1‑t^i)
 * Range:     u8 n <= 25, u16 n <= 73, u32 n <= 371, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case immediately. */
    if (n == 0)
        return (seq_t)1;

    /* Allocate space for the DP table: dp[0 … n]. */
    size_t len = (size_t)n + 1;
    seq_t *dp = (seq_t*)malloc(len * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;      /* Out of memory – unlikely in tests. */

    /* Initialize the table. */
    memset(dp, 0, len * sizeof(seq_t));
    dp[0] = (seq_t)1;

    /* Classic unbounded‑knapsack DP to count partitions with parts 2–10. */
    for (idx_t part = 2; part <= 10; ++part) {
        for (idx_t j = part; j <= n; ++j) {
            dp[j] += dp[j - part];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
