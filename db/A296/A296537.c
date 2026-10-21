/*
 * A296537  Number of nX4 0..1 arrays with each 1 horizontally, vertically or
 *          antidiagonally adjacent to 2 or 4 neighboring 1s.
 * https://oeis.org/A296537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary arrays such that every 1 has exactly
 *            2 or 4 horizontally, vertically or antidiagonally adjacent 1s.
 *            Computed by a transfer‑matrix DP over rows
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   valid_row_state
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/valid_row_state.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                         /* offset is 1, but return 0 for n=0 */

    static seq_t dp[16][16];
    static seq_t ndp[16][16];

    /* initialise dp with a single state: rows -1 and 0 are zero */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            dp[i][j] = 0;
    dp[0][0] = 1;

    /* iterate over each actual row of the grid */
    for (idx_t step = 1; step <= n; ++step) {
        /* clear ndp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                ndp[i][j] = 0;

        for (int pp = 0; pp < 16; ++pp) {          /* previous of previous row */
            for (int pr = 0; pr < 16; ++pr) {      /* previous row */
                seq_t ways = dp[pp][pr];
                if (ways == 0)
                    continue;
                for (int cu = 0; cu < 16; ++cu) {  /* current row candidate */
                    if (valid_row_state((uint8_t)pp, (uint8_t)pr, (uint8_t)cu))
                        ndp[pr][cu] += ways;
                }
            }
        }

        /* move ndp to dp for next iteration */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* finalize by attaching the trailing zero row */
    seq_t answer = 0;
    for (int pp = 0; pp < 16; ++pp) {
        for (int pr = 0; pr < 16; ++pr) {
            seq_t ways = dp[pp][pr];
            if (ways == 0)
                continue;
            if (valid_row_state((uint8_t)pp, (uint8_t)pr, 0))
                answer += ways;
        }
    }

    return answer;
}
