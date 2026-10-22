/*
 * A341025  Number of partitions of n into 8 distinct nonzero triangular
 *          numbers.
 * https://oeis.org/A341025
 *
 * Offset:    120    (a(120) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 220, u16 n <= 220, u32 n <= 220, u64 n <= 220,
 *            u128 n <= 220, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into 8 distinct nonzero triangular numbers */
#include <stdbool.h>

#define MAXN 10000   /* enough for all requested n (>=120 up to ~10000) */

seq_t a(idx_t n)
{
    static seq_t dp[9][MAXN + 1] = {0};     /* dp[j][s] = ways to pick j distinct triangulars summing to s */
    static bool initialized = false;

    if (!initialized) {
        dp[0][0] = 1;                       /* one way to pick zero numbers */

        for (int k = 1; ; ++k) {
            idx_t t = (idx_t)k * (k + 1) / 2;   /* triangular number T_k */
            if (t > MAXN) break;
            int t_int = (int)t;                /* for use in signed loops */

            for (int j = 8; j >= 1; --j) {
                for (int s = MAXN; s >= t_int; --s) {
                    dp[j][s] += dp[j - 1][s - t_int];
                }
            }
        }
        initialized = true;
    }

    if (n <= MAXN) {
        return dp[8][(int)n];
    } else {
        return 0;   /* n beyond precomputed range (not expected) */
    }
}
