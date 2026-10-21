/*
 * A266773  Molien series for invariants of finite Coxeter group D_10
 *          (bisected).
 * https://oeis.org/A266773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n using parts
 *            {1,2,3,4,5,5,6,7,8,9} (i.e. the coefficient of x^n in 1 /
 *            ((1-x^5)^2 * ∏_{j=1..9, j≠5} (1-x^j))
 * Range:     u8 n <= 15, u16 n <= 46, u32 n <= 217, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static const idx_t parts[10] = {1, 2, 3, 4, 5, 5, 6, 7, 8, 9};

static seq_t *dp = NULL;      /* dynamically grown DP array */
static idx_t  dp_size = 0;    /* current allocated size of dp */

seq_t a(idx_t n)
{
    /* ensure dp is large enough */
    if (dp == NULL || n >= dp_size) {
        idx_t new_size = (n + 1) * 2;
        if (new_size < n + 1) new_size = n + 1;  /* protect overflow */

        seq_t *new_dp = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (dp) {
            memcpy(new_dp, dp, dp_size * sizeof(seq_t));
            free(dp);
        } else {
            new_dp[0] = 0; /* dp[0] not used prior to this call */
        }
        dp = new_dp;
        dp_size = new_size;
    }

    /* initialise dp[0] and clear 1..n */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* classic unbounded‑knapsack / partition DP */
    for (idx_t pi = 0; pi < 10; ++pi) {
        idx_t part = parts[pi];
        for (idx_t j = part; j <= n; ++j)
            dp[j] += dp[j - part];
    }

    return dp[n];
}
