/*
 * A279220  Expansion of Product_{k>=1} 1/(1 - x^(k*(k+1)*(2*k+1)/6)).
 * https://oeis.org/A279220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of partitions of n into square pyramidal numbers
 *            (A000330) a(n) = coefficient of x^n in Product_{k>=1} 1/(1 -
 *            x^{k*(k+1)*(2*k+1)/6})
 * Range:     u8 n <= 113, u16 n <= 472, u32 n <= 2612, u64 n <= 9173,
 *            u128 n <= 9173, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;
static idx_t dp_size = 0;

seq_t a(idx_t n) {
    /* Ensure the DP array is large enough */
    if (dp == NULL || dp_size <= n) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (tmp == NULL) { /* allocation failed – return 0 as a safe fallback */
            return 0;
        }
        dp = tmp;
        dp_size = new_size;
    }

    /* Reset values up to n */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;

    dp[0] = 1;  /* empty partition */

    /* Iterate over all square pyramidal numbers p <= n */
    for (idx_t k = 1; ; ++k) {
        idx_t p = k * (k + 1) * (2 * k + 1) / 6;  /* k*(k+1)*(2k+1)/6 */
        if (p > n)
            break;
        for (idx_t i = p; i <= n; ++i)
            dp[i] += dp[i - p];
    }

    return dp[n];
}
