/*
 * A258548  Number of (n+1) X (2+1) 0..1 arrays with every 2 X 2 subblock
 *          ne-sw antidiagonal difference nondecreasing horizontally and nw+se
 *          diagonal sum nondecreasing vertically.
 * https://oeis.org/A258548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 32, u64 n <= 73, u128 n <= 155,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: dynamic programming over rows.
   State = two most recent rows (each a 3-bit pattern).
   Transition from (prev_prev,prev) to (prev,curr) is allowed iff :
     - horizontal constraint on pair (prev,curr) holds
     - vertical constraint on triple (prev_prev,prev,curr) holds
   For n+1 rows the answer is the sum over all states after processing the last row. */

#include <stdbool.h>

/* DP implementation for A258548 */
seq_t a(idx_t n) {
    if (n == 0) /* offset is 1; undefined for n=0, return 0 for safety */
        return (seq_t)0;

    /* Precompute all pair and triple compatibility tables */
    static bool pair_ok[8][8];
    static bool triple_ok[8][8][8];
    static bool done = false;
    if (!done) {
        for (int p = 0; p < 8; ++p) {
            int bp0 = (p >> 0) & 1;
            int bp1 = (p >> 1) & 1;
            int bp2 = (p >> 2) & 1;
            for (int q = 0; q < 8; ++q) {
                int bq0 = (q >> 0) & 1;
                int bq1 = (q >> 1) & 1;
                int bq2 = (q >> 2) & 1;

                /* horizontal constraint for pair (p,q) */
                int diff0 = bp1 - bq0;
                int diff1 = bp2 - bq1;
                pair_ok[p][q] = (diff0 <= diff1);

                for (int r = 0; r < 8; ++r) {
                    int br0 = (r >> 0) & 1;
                    int br1 = (r >> 1) & 1;
                    int br2 = (r >> 2) & 1;

                    /* vertical constraints for triple (p,q,r) */
                    int sum0_prev = bp0 + bq1;
                    int sum0_curr = bq0 + br1;
                    int sum1_prev = bp1 + bq2;
                    int sum1_curr = bq1 + br2;
                    triple_ok[p][q][r] =
                        (sum0_prev <= sum0_curr) && (sum1_prev <= sum1_curr);
                }
            }
        }
        done = true;
    }

    /* DP tables: dp[prev_prev][prev] = number of ways to reach this state */
    seq_t dp[8][8];
    seq_t dp_next[8][8];
    /* Initialize dp for the first two rows */
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            dp[i][j] = pair_ok[i][j] ? (seq_t)1 : (seq_t)0;

    /* Process remaining rows: total rows = n+1, we already handled 2 */
    idx_t total_rows = n + 1;
    for (idx_t step = 2; step < total_rows; ++step) {
        /* Zero dp_next */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp_next[i][j] = (seq_t)0;

        /* Transition */
        for (int p = 0; p < 8; ++p) {
            for (int q = 0; q < 8; ++q) {
                seq_t ways = dp[p][q];
                if (ways == (seq_t)0) continue;
                for (int r = 0; r < 8; ++r) {
                    if (!pair_ok[q][r]) continue;
                    if (!triple_ok[p][q][r]) continue;
                    dp_next[q][r] += ways;
                }
            }
        }

        /* copy dp_next to dp */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp[i][j] = dp_next[i][j];
    }

    /* Sum over final states to get the answer */
    seq_t result = (seq_t)0;
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            result += dp[i][j];

    return result;
}
