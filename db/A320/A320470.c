/*
 * A320470  Number of partitions of n such that the successive differences of
 *          consecutive parts are strictly decreasing.
 * https://oeis.org/A320470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n with strictly decreasing
 *            successive differences of consecutive parts. The algorithm
 *            enumerates partitions by their length k, smallest part x, and a
 *            strictly decreasing sequence of differences d_2 > d_3 > ... >
 *            d_k ≥ 0. It is based on the identity n = k*x + Σ_{i=2}...
 * Range:     u8 n <= 61, u16 n <= 291, u32 n <= 1622, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#if defined(__cplusplus)
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* Empty partition is counted for n == 0 */
    if (n == 0) return (seq_t)1;

    seq_t total = 0;

    /* Determine maximum possible number of parts k */
    idx_t k = 1;
    while (true) {
        idx_t L = k - 1;
        /* C = L*(L+1)*(L-1)/3  (minimum weighted sum of differences) */
        unsigned __int128 prod = (unsigned __int128)L * (L + 1) * (L - 1);
        idx_t C = (idx_t)(prod / 3);
        if (n < C + k) break;        /* n is too small to accommodate k parts   */
        ++k;
    }
    --k;                                /* final valid k value */

    for (idx_t kk = 1; kk <= k; ++kk) {
        idx_t L = kk - 1;
        unsigned __int128 prod = (unsigned __int128)L * (L + 1) * (L - 1);
        idx_t C = (idx_t)(prod / 3);          /* minimal weighted sum of diffs */

        idx_t target = n - C;                 /* must be ≥ kk */
        if (target < kk) continue;           /* no solutions for this kk */

        /* Allocate array for DP: ways[s] = number of ways to write s as
           Σ_j j * f_j with f_j non‑decreasing (unbounded knapsack).   */
#if defined(__cplusplus)
        std::vector<seq_t> ways(target + 1, (seq_t)0);
#else
        seq_t *ways = (seq_t*)malloc((size_t)(target + 1) * sizeof(seq_t));
        if (!ways) return total;             /* allocation failed */
        for (idx_t i = 0; i <= target; ++i) ways[i] = 0;
#endif
        ways[0] = (seq_t)1;

        if (L > 0) {
            /* Compute weights w_t = (L*(L+1) - (t-1)*t) / 2   for t=1..L   */
            for (idx_t t = 1; t <= L; ++t) {
                unsigned __int128 num = (unsigned __int128)L * (L + 1)
                                        - (unsigned __int128)(t - 1) * t;
                idx_t w = (idx_t)(num / 2);
                for (idx_t s = w; s <= target; ++s) {
                    ways[s] += ways[s - w];
                }
            }
        }

        /* Sum contributions for all possible smallest parts x ≥ 1 */
        idx_t max_x = target / kk;
        for (idx_t x = 1; x <= max_x; ++x) {
            idx_t idx = target - kk * x;
            total += ways[idx];
        }

#if !defined(__cplusplus)
        free(ways);
#endif
    }
    return total;
}
