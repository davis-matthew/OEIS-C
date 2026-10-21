/*
 * A276643  Number of distinct 4-digit vehicle plates whose sum is n.
 * https://oeis.org/A276643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   a(n) = coefficient of (1 + x + … + x^9)^4 in x^n, 0 ≤ n ≤ 36
 * Range:     u8 n <= 9, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Cache of the 37 values, computed once. */
    static seq_t cache[37];
    static unsigned char initialized = 0;

    if (!initialized) {
        /* Temporary DP arrays: 0‑36 sums. */
        seq_t dp[37] = {0};
        dp[0] = 1;                    /* one way to make sum 0 with zero digits */

        for (idx_t pos = 0; pos < 4; ++pos) {   /* for each of the 4 digits */
            seq_t next[37] = {0};
            for (idx_t sum = 0; sum <= 36; ++sum) {
                seq_t ways = dp[sum];
                if (ways == 0) continue;
                for (idx_t dig = 0; dig <= 9; ++dig) {
                    idx_t ns = sum + dig;
                    if (ns > 36) break;
                    next[ns] += ways;
                }
            }
            for (idx_t sum = 0; sum <= 36; ++sum)
                dp[sum] = next[sum];
        }

        for (idx_t sum = 0; sum <= 36; ++sum)
            cache[sum] = dp[sum];

        initialized = 1;
    }

    return (n <= 36) ? cache[(idx_t)n] : 0;
}
