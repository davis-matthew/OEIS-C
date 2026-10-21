/*
 * A250613  Number of (n+1)X(1+1) 0..2 arrays with nondecreasing maximum of
 *          every two consecutive values in every row and column.
 * https://oeis.org/A250613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 28, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A250613: number of (n+1)×2 arrays over {0,1,2} with the
   non‑decreasing maximum of every two consecutive values in each row
   and column.  A linear DP with 81 states is used, so the algorithm
   runs in O(n) time and O(1) memory (apart from the state array). */

#include <stddef.h>
#include <stdbool.h>

enum { ROWS = 9, STATES = 81 };

static const int row_vals[ROWS][2] = {
    {0,0},{0,1},{0,2},
    {1,0},{1,1},{1,2},
    {2,0},{2,1},{2,2}
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* sequence defined for n ≥ 1 only */

    /* Transition table: trans[s][r] = next state for current state s
       if the new row is r.  -1 means the transition is forbidden. */
    static int trans[STATES][ROWS];
    static bool trans_init = false;

    if (!trans_init) {
        for (int s = 0; s < STATES; ++s) {
            int sr   = (s / 27) % 3;          /* previous row[0]   */
            int ss   = (s / 9)  % 3;          /* previous row[1]   */
            int ms1  = (s / 3)  % 3;          /* previous column 0 max */
            int ms2  =  s        % 3;          /* previous column 1 max */

            for (int r = 0; r < ROWS; ++r) {
                int cr = row_vals[r][0];
                int dr = row_vals[r][1];

                int new_ms1 = (cr > sr) ? cr : sr;
                int new_ms2 = (dr > ss) ? dr : ss;

                /* enforce non‑decreasing column maximums */
                if (new_ms1 >= ms1 && new_ms2 >= ms2) {
                    int t = (cr * 27) + (dr * 9) + (new_ms1 * 3) + new_ms2;
                    trans[s][r] = t;
                } else {
                    trans[s][r] = -1;
                }
            }
        }
        trans_init = true;
    }

    /* Dynamic programming tables */
    seq_t dp[STATES]  = {0};
    seq_t ndp[STATES] = {0};

    /* Initialize after the first row.  For the first row the previous
       column maximums are implicitly 0. */
    for (int r = 0; r < ROWS; ++r) {
        int sr = row_vals[r][0];
        int ss = row_vals[r][1];
        int state = (sr * 27) + (ss * 9);   /* ms1 = ms2 = 0 */
        dp[state] = 1;
    }

    /* Each additional row (there are n more rows after the first one) */
    for (idx_t step = 2; step <= n + 1; ++step) {
        for (int i = 0; i < STATES; ++i) ndp[i] = 0;

        for (int s = 0; s < STATES; ++s) {
            seq_t cur = dp[s];
            if (!cur) continue;
            for (int r = 0; r < ROWS; ++r) {
                int ns = trans[s][r];
                if (ns >= 0) ndp[ns] += cur;
            }
        }

        for (int i = 0; i < STATES; ++i) dp[i] = ndp[i];
    }

    /* Sum all counts for the final row */
    seq_t result = 0;
    for (int i = 0; i < STATES; ++i) result += dp[i];
    return result;
}
