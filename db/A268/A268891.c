/*
 * A268891  Number of 6Xn binary arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling two
 *          exactly once.
 * https://oeis.org/A268891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 6×n binary arrays with exactly one horizontally
 *            * or antidiagonally adjacent pair of 1s
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   count_new_pairs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_new_pairs.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* For n=1 there is no adjacency possible */

    /* dp[pattern][pairs_so_far] where pairs_so_far is 0 or 1 */
    seq_t dp_prev[64][2] = {0};
    seq_t dp_next[64][2] = {0};

    /* Initialize with first column: all 64 patterns possible, no pairs yet */
    for (int s = 0; s < 64; ++s) dp_prev[s][0] = 1;

    /* Iterate over remaining columns */
    for (idx_t col = 1; col < n; ++col) {
        /* reset dp_next */
        for (int s = 0; s < 64; ++s) {
            dp_next[s][0] = 0;
            dp_next[s][1] = 0;
        }

        /* Transitions from previous column patterns to current ones */
        for (int p = 0; p < 64; ++p) {
            /* Skip if this state has zero ways in both pair counts */
            if (dp_prev[p][0] == 0 && dp_prev[p][1] == 0) continue;

            for (int q = 0; q < 64; ++q) {
                int new_pairs = count_new_pairs((unsigned int)p, (unsigned int)q);
                if (new_pairs > 1) continue; /* cannot end with exactly one pair */

                /* From state with 0 pairs so far */
                if (dp_prev[p][0]) {
                    dp_next[q][new_pairs] += dp_prev[p][0];
                }

                /* From state with 1 pair already counted */
                if (dp_prev[p][1] && new_pairs == 0) {
                    dp_next[q][1] += dp_prev[p][1];
                }
            }
        }

        /* Move next to prev for next iteration */
        for (int s = 0; s < 64; ++s) {
            dp_prev[s][0] = dp_next[s][0];
            dp_prev[s][1] = dp_next[s][1];
        }
    }

    /* Sum all patterns that have exactly one pair after processing all columns */
    seq_t ans = 0;
    for (int s = 0; s < 64; ++s) ans += dp_prev[s][1];
    return ans;
}
