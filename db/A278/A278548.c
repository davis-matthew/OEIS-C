/*
 * A278548  Number of nX2 0..3 arrays with rows and columns in lexicographic
 *          nondecreasing order but with exactly two mistakes.
 * https://oeis.org/A278548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 25, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: dynamic‑programming count of n×2 arrays over {0,1,2,3}
   with rows and columns lexicographically nondecreasing,
   except exactly two violations (row comparisons or the column comparison). */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    enum { M = 4, PAIRS = M*M };          /* 16 possible rows (a,b) */

    static bool row_fail[PAIRS][PAIRS];
    static int  next_ord[3][PAIRS];       /* transitions for column state */
    static bool tables_computed = false;
    if (!tables_computed) {
        for (int x = 0; x < M; ++x)
            for (int y = 0; y < M; ++y) {
                int id = x * M + y;
                for (int u = 0; u < M; ++u)
                    for (int v = 0; v < M; ++v) {
                        int id2 = u * M + v;
                        row_fail[id][id2] = (x > u) || (x == u && y > v);
                    }
                /* column status transition from state 0 */
                if (x < y)       next_ord[0][id] = 1;   /* column 1 < column 2  */
                else if (x > y)  next_ord[0][id] = 2;   /* column 1 > column 2  */
                else             next_ord[0][id] = 0;   /* equal so far        */
                /* states 1 and 2 stay unchanged */
                next_ord[1][id] = 1;
                next_ord[2][id] = 2;
            }
        tables_computed = true;
    }

    /* DP state: [last_row][row_fail_count][column_state] */
    seq_t dp[PAIRS][3][3];
    seq_t ndp[PAIRS][3][3];
    for (int r = 0; r < PAIRS; ++r)
        for (int f = 0; f < 3; ++f)
            for (int s = 0; s < 3; ++s)
                dp[r][f][s] = 0;

    /* first row */
    for (int x = 0; x < M; ++x)
        for (int y = 0; y < M; ++y) {
            int id = x * M + y;
            int s  = next_ord[0][id];
            dp[id][0][s] = dp[id][0][s] + 1;
        }

    /* process remaining rows */
    for (idx_t row = 1; row < n; ++row) {
        for (int r = 0; r < PAIRS; ++r)
            for (int f = 0; f < 3; ++f)
                for (int s = 0; s < 3; ++s)
                    ndp[r][f][s] = 0;

        for (int prev = 0; prev < PAIRS; ++prev)
            for (int f = 0; f < 3; ++f)
                for (int s = 0; s < 3; ++s) {
                    seq_t ways = dp[prev][f][s];
                    if (ways == 0) continue;
                    for (int next = 0; next < PAIRS; ++next) {
                        int new_f = f + (row_fail[prev][next] ? 1 : 0);
                        if (new_f > 2) continue;
                        int new_s = next_ord[s][next];
                        ndp[next][new_f][new_s] = ndp[next][new_f][new_s] + ways;
                    }
                }

        for (int r = 0; r < PAIRS; ++r)
            for (int f = 0; f < 3; ++f)
                for (int s = 0; s < 3; ++s)
                    dp[r][f][s] = ndp[r][f][s];
    }

    /* sum solutions with exactly two violations */
    seq_t ans = 0;
    for (int r = 0; r < PAIRS; ++r)
        for (int f = 0; f < 3; ++f)
            for (int s = 0; s < 3; ++s) {
                seq_t ways = dp[r][f][s];
                if (ways == 0) continue;
                int col_fail = (s == 2) ? 1 : 0;
                if (f + col_fail == 2)
                    ans = ans + ways;
            }

    return ans;
}
