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
 * Validated: standard, pass, 40 terms through n = 208 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Check whether the middle row `mid` (0..15) satisfies the
   adjacency condition when the previous row is `prev`
   and the next row is `next`.  */
static inline bool valid_row_state(uint8_t prev, uint8_t mid, uint8_t next)
{
    for (int c = 0; c < 4; ++c) {
        int bit = 1 << c;
        if (!(mid & bit))
            continue;                     /* no requirement for a 0 cell */
        int count = 0;
        /* vertical neighbors */
        if (prev & bit) count++;
        if (next & bit) count++;
        /* horizontal neighbors */
        if (c > 0 && (mid & (1 << (c - 1)))) count++;
        if (c < 3 && (mid & (1 << (c + 1)))) count++;
        /* anti‑diagonal neighbors */
        if (c < 3 && (prev & (1 << (c + 1)))) count++;
        if (c > 0 && (next & (1 << (c - 1)))) count++;
        if (count != 2 && count != 4)
            return false;
    }
    return true;
}

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
