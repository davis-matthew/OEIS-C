/*
 * A319799  Number of partitions of 2n into exactly n positive triangular
 *          numbers.
 * https://oeis.org/A319799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts of the form T_k−1 =
 *            k(k+1)/2 − 1 (k ≥ 2). This equals the number of partitions of 2n
 *            into exactly n triangular numbers. The function uses a standard
 *            coin‑change dynamic programming
 * Range:     u8 n <= 73, u16 n <= 220, u32 n <= 810, u64 n <= 3644,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   // malloc, free

static seq_t *dp = NULL;   // DP table: dp[s] = number of partitions of s
static idx_t max_n = 0;    // largest n for which dp has been computed

seq_t a(idx_t n)
{
    if (n == 0) return 1;   // dp[0] is 1 once the table is built

    /* Recompute table if needed */
    if (n > max_n) {
        idx_t new_max = n;
        size_t len = (size_t)new_max + 1;
        seq_t *newdp = (seq_t *)malloc(len * sizeof(seq_t));
        if (!newdp) return 0;   // allocation failed

        /* Initialize */
        for (size_t i = 0; i < len; ++i)
            newdp[i] = 0;
        newdp[0] = 1;

        /* Generate coin denominations: c = T_j − 1 = j(j+1)/2 − 1 for j ≥ 2 */
        for (idx_t j = 2;; ++j) {
            idx_t t = j * (j + 1) / 2;   /* triangular number */
            idx_t c = t - 1;             /* coin value */
            if (c > new_max) break;
            for (idx_t s = c; s <= new_max; ++s) {
                newdp[(size_t)s] = newdp[(size_t)s] + newdp[(size_t)(s - c)];
            }
        }

        free(dp);
        dp = newdp;
        max_n = new_max;
    }

    return dp[(size_t)n];
}
