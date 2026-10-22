/*
 * A383079  Number of n-digit positive integers where all pairs of consecutive
 *          digits have a difference of at least 7.
 * https://oeis.org/A383079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 26, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of n‑digit positive integers in which every pair of
   consecutive digits differ by at least 7.  The computation uses a
   simple dynamic programming recurrence:
 
   dp[i][d] = number of length‑i numbers ending with digit d
   (0 ≤ d ≤ 9) that satisfy the adjacency condition.
   For i = 1 the only admissible leading digits are 1…9.
   Transition:
        dp[i][0] = dp[i‑1][7] + dp[i‑1][8] + dp[i‑1][9]
        dp[i][1] = dp[i‑1][8] + dp[i‑1][9]
        dp[i][2] = dp[i‑1][9]
        dp[i][3] = dp[i‑1][4] = dp[i‑1][5] = dp[i‑1][6] = 0
        dp[i][7] = dp[i‑1][0]
        dp[i][8] = dp[i‑1][0] + dp[i‑1][1]
        dp[i][9] = dp[i‑1][0] + dp[i‑1][1] + dp[i‑1][2]
   The answer a(n) is ∑_{d=0}^{9} dp[n][d].
*/

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the obligatory offset.  For n == 0 the sequence is undefined;
       we return 0 to avoid undefined behaviour. */
    if (n == 0)
        return (seq_t)0;

    /* DP tables for the current and next digit position. */
    seq_t dp[10]   = {0};
    seq_t ndp[10] = {0};

    /* Initialize for length 1 (leading digit cannot be 0). */
    dp[1] = dp[2] = dp[3] = dp[4] = dp[5] = dp[6] = dp[7] = dp[8] = dp[9] = (seq_t)1;
    dp[0] = (seq_t)0;            /* leading zero is forbidden */

    /* Return for n == 1. */
    if (n == 1) {
        seq_t sum = dp[1] + dp[2] + dp[3] + dp[4] + dp[5] +
                    dp[6] + dp[7] + dp[8] + dp[9];
        return sum;
    }

    /* Iterate from length 2 up to n. */
    for (idx_t len = 2; len <= n; ++len) {
        /* Compute ndp based on the transition rules. */
        ndp[0] = dp[7] + dp[8] + dp[9];
        ndp[1] = dp[8] + dp[9];
        ndp[2] = dp[9];
        ndp[3] = ndp[4] = ndp[5] = ndp[6] = (seq_t)0;
        ndp[7] = dp[0];
        ndp[8] = dp[0] + dp[1];
        ndp[9] = dp[0] + dp[1] + dp[2];

        /* Copy ndp back into dp for the next iteration. */
        for (int d = 0; d < 10; ++d)
            dp[d] = ndp[d];
    }

    /* Sum all digits for the final answer. */
    seq_t result = (seq_t)0;
    for (int d = 0; d < 10; ++d)
        result += dp[d];
    return result;
}
