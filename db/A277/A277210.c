/*
 * A277210  Expansion of Product_{k>=1} 1/(1 - x^(3*k+1)).
 * https://oeis.org/A277210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts >1 and congruent to
 *            1 mod 3 (parts are 4,7,10,13,...). This is computed by the
 *            classic coin‑change DP using the generating function ∏_{k≥1}
 *            1/(1 - x^(3k+1))
 * Range:     u8 n <= 70, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* The function is memoised over calls; when a larger n is requested the
   dynamic programme is recomputed up to that value. */
seq_t a(idx_t n)
{
    /* static lazy‑initialised DP table */
    static seq_t *dp = NULL;
    static idx_t max_n = 0;

    /* if we need more values, recompute the table */
    if (n > max_n || dp == NULL) {
        idx_t new_n = n;
        if (dp != NULL)
            free(dp);                       /* free old table */
        dp = (seq_t*)malloc(sizeof(seq_t) * (size_t)(new_n + 1));
        if (dp == NULL)                   /* allocation failure */
            return (seq_t)0;

        /* initialise table: dp[0] = 1, others 0 */
        for (idx_t i = 0; i <= new_n; ++i)
            dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;

        /* coin‑change DP */
        for (idx_t coin = 4; coin <= new_n; coin += 3) {
            for (idx_t s = coin; s <= new_n; ++s) {
                dp[s] = dp[s] + dp[s - coin];
            }
        }
        max_n = new_n;
    }
    return dp[n];
}
