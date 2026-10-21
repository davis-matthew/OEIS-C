/*
 * A250976  Number of (n+1)X(2+1) 0..2 arrays with no 2X2 subblock having the
 *          sum of its diagonal elements less than the absolute difference of
 *          its antidiagonal elements.
 * https://oeis.org/A250976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays over {0,1,2} with * the property
 *            that for every adjacent pair of rows (r,r+1) and for * each
 *            column pair (c,c+1) the 2×2 subblock satisfies * (top-left +
 *            bottom-right) ≥ |(top-right – bottom-left)|. * The sequence is
 *            computed via DP over all 27 possible rows
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* 27 possible rows: encode in base‑3 (d0 + 3*d1 + 9*d2) */
    static bool allowed[27][27] = {0};
    static bool table_computed = false;
    if (!table_computed) {
        /* Pre‑compute the adjacency matrix */
        for (int s1 = 0; s1 < 27; ++s1) {
            int x0 = s1 % 3;
            int x1 = (s1 / 3) % 3;
            int x2 = s1 / 9;
            for (int s2 = 0; s2 < 27; ++s2) {
                int y0 = s2 % 3;
                int y1 = (s2 / 3) % 3;
                int y2 = s2 / 9;
                /* Check the two 2×2 subblocks */
                int diag0 = x0 + y1;
                int ant0  = x1 + y0;
                int diag1 = x1 + y2;
                int ant1  = x2 + y1;
                int ok0 = diag0 >= (x1 > y0 ? x1 - y0 : y0 - x1);
                int ok1 = diag1 >= (x2 > y1 ? x2 - y1 : y1 - x2);
                allowed[s1][s2] = (ok0 && ok1);
            }
        }
        table_computed = true;
    }

    /* DP over rows: dp[state] = number of ways to reach current row state */
    seq_t dp[27];
    seq_t next[27];
    for (int i = 0; i < 27; ++i) dp[i] = 1;  /* first row can be anything */

    /* Number of transitions equals n (since we have n+1 rows) */
    for (uint64_t step = 0; step < n; ++step) {
        for (int i = 0; i < 27; ++i) next[i] = 0;
        for (int s1 = 0; s1 < 27; ++s1) {
            if (dp[s1] == 0) continue;
            for (int s2 = 0; s2 < 27; ++s2) {
                if (allowed[s1][s2]) next[s2] += dp[s1];
            }
        }
        /* swap dp and next */
        for (int i = 0; i < 27; ++i) dp[i] = next[i];
    }

    /* Sum all possible final states */
    seq_t total = 0;
    for (int i = 0; i < 27; ++i) total += dp[i];
    return total;
}
