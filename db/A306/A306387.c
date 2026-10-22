/*
 * A306387  Number of partitions of sigma(n) into divisors of n, where sigma =
 *          A000203.
 * https://oeis.org/A306387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of σ(n) into divisors of n, computed
 *            by the classic coin‑change dynamic programming algorithm
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 119, u64 n <= 359,
 *            u128 n <= 1679, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, free */
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* Generate all divisors of n.  The maximum number of divisors for
       n ≤ 10 000 is well below 200, so a small static array suffices. */
    idx_t divs[256];
    size_t dcnt = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            divs[dcnt++] = i;
            if (i != n / i) {
                divs[dcnt++] = n / i;
            }
        }
    }

    /* Compute σ(n) = sum of divisors. */
    idx_t sigma = 0;
    for (size_t k = 0; k < dcnt; ++k) {
        sigma += divs[k];
    }

    /* Allocate DP table.  Use C++ vector where available to keep the
       code compilable in C++ and plain malloc in C. */
#ifdef __cplusplus
    std::vector<seq_t> dp(sigma + 1);
#else
    seq_t *dp = (seq_t *)malloc((sigma + 1) * sizeof(seq_t));
#endif

    /* Initialise dp[0] = 1, others 0. */
    for (idx_t i = 0; i <= sigma; ++i) {
        dp[i] = 0;
    }
    dp[0] = 1;

    /* Classic unbounded knapsack (coin change) DP. */
    for (size_t d = 0; d < dcnt; ++d) {
        idx_t coin = divs[d];
        for (idx_t s = coin; s <= sigma; ++s) {
            dp[s] += dp[s - coin];
        }
    }

    /* Extract result and clean up. */
    seq_t result = dp[sigma];
#ifdef __cplusplus
    /* Nothing to free for vector. */
#else
    free(dp);
#endif
    return result;
}
