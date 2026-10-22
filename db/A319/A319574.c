/*
 * A319574  A(n, k) = [x^k] JacobiTheta3(x)^n, square array read by descending
 *          antidiagonals, A(n, k) for n >= 0 and k >= 0.
 * https://oeis.org/A319574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the coefficient of x^k in JacobiTheta3(x)^n, i.e., the
 *            number of ways to express k as a sum of n squares, read by
 *            descending antidiagonals
 * Range:     u8 n <= 61, u16 n <= 199, u32 n <= 613, u64 n <= 2181,
 *            u128 n <= 8073, big any n
 * Helpers:   index_to_nk
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/index_to_nk.h"

static seq_t dp[300][300];  /* DP[n][k] – works for all terms up to ~10 000 indices. */
static int dp_max = -1;     /* Highest diagonal index computed so far. */

/* Recompute the table for all 0 ≤ n, k ≤ d. */
static void recompute_dp(int d)
{
    for (int n = 0; n <= d; ++n) {
        for (int k = 0; k <= d; ++k) {
            if (n == 0) {
                dp[0][k] = (k == 0 ? (seq_t)1 : (seq_t)0);
            } else {
                seq_t sum = dp[n-1][k];              /* m = 0 term. */
                for (int m = 1; (int)m*m <= k; ++m) {
                    seq_t add = dp[n-1][k - m*m];
                    sum += add + add;               /* ±m give two contributions. */
                }
                dp[n][k] = sum;
            }
        }
    }
    dp_max = d;
}

seq_t a(idx_t n)
{
    int n_needed, k_needed;
    index_to_nk(n, &n_needed, &k_needed);
    int required_d = n_needed > k_needed ? n_needed : k_needed;
    if (required_d > dp_max)
        recompute_dp(required_d);
    return dp[n_needed][k_needed];
}
