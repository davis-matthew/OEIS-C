/*
 * A256525  Number of partitions of 3n into at most 5 parts.
 * https://oeis.org/A256525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A001401(3n) – number of partitions of 3*n into at most 5
 *            parts
 * Range:     u8 n <= 7, u16 n <= 36, u32 n <= 622, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute a(n) - partitions of 3*n into at most 5 parts */
seq_t a(idx_t n)
{
    /* we need the value for 3*n */
    idx_t target = (idx_t)((seq_t)n * 3);

    /* static cache of the dynamic programming table */
    static seq_t *dp = NULL;          /* array of seq_t of size current_max+1 */
    static idx_t current_max = 0;     /* current maximum index stored in dp */

    /* if we haven't computed up to the required index yet, recompute */
    if (dp == NULL || target > current_max) {
        /* allocate new table up to target */
        seq_t *newdp = (seq_t *)calloc((size_t)(target + 1), sizeof(seq_t));
        if (newdp == NULL) { /* allocation failed – return 0 (undefined) */ }
        newdp[0] = 1; /* there's exactly one partition of 0 */

        /* classic coin‑change DP with coins 1..5.
           This counts partitions with parts <=5, which equals partitions
           into at most 5 parts. */
        for (idx_t coin = 1; coin <= 5; ++coin) {
            for (idx_t i = coin; i <= target; ++i) {
                newdp[i] = newdp[i] + newdp[i - coin];
            }
        }

        /* free old table and store the new one */
        free(dp);
        dp = newdp;
        current_max = target;
    }

    return dp[target];
}
