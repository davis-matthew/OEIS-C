/*
 * A394243  Expansion of Product_{k>=1} 1/(1 - x^(2*k^2 - 1)).
 * https://oeis.org/A394243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 449, u32 n <= 1960, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = coefficient of x^n in ∏_{k≥1} 1/(1 – x^(2k^2 – 1))
 *              (partition function with part sizes 1,7,17,31,49,…)
 *
 * The algorithm uses the classic coin‑change DP:
 *   dp[0] = 1;  dp[i] = Σ dp[i‑p] over all allowed parts p ≤ i
 * This is O(n × #parts) ≈ O(n √n), fast enough for n ≤ 10000.
 */
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    // C++: use std::vector to ensure proper construction of arbitrary‑precision
    std::vector<seq_t> dp(n + 1);
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    for (idx_t k = 1; ; ++k) {
        idx_t part = 2 * k * k - 1;   // part size 2k^2 – 1
        if (part > n) break;
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }
    return dp[n];
#else
    // C: allocate raw array; seq_t is an integer type here
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0;          // allocation failure
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    for (idx_t k = 1; ; ++k) {
        idx_t part = 2 * k * k - 1;   // part size 2k^2 – 1
        if (part > n) break;
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }
    seq_t result = dp[n];
    free(dp);
    return result;
#endif
}
