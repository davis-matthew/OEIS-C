/*
 * A278267  Number of n X 2 0..3 arrays with every element plus 1 mod 4 equal
 *          to some element at offset (-1,-1) (-1,0) (-1,1) (0,-1) (0,1)
 *          (1,-1) or (1,0), with upper left element zero.
 * https://oeis.org/A278267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   left_val, right_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A278267: number of n×2 arrays over {0,1,2,3} with the
    neighbour condition described in OEIS.
    Implementation uses a transfer–matrix DP over rows:
    the validity of a middle row depends only on the
    previous, current and next rows, so a state of two rows
    suffices.  All calculations are done with the build‑defined
    type seq_t, which may be an arbitrary‑precision integer.  */
#include <stdbool.h>
#include "../utils/left_val.h"
#include "../utils/right_val.h"

#define MISSING 16

/*  Check whether the current row satisfies the neighbour condition
    given the (optional) previous and next rows.  */
static bool row_ok(int prevCode, int currCode, int nextCode)
{
    int curr[2] = { left_val(currCode), right_val(currCode) };
    int prev[2] = { -1, -1 };
    int next[2] = { -1, -1 };
    bool prevExist = (prevCode != MISSING);
    bool nextExist = (nextCode != MISSING);

    if (prevExist) {
        prev[0] = left_val(prevCode);
        prev[1] = right_val(prevCode);
    }
    if (nextExist) {
        next[0] = left_val(nextCode);
        next[1] = right_val(nextCode);
    }

    for (int col = 0; col < 2; ++col) {
        int target = (curr[col] + 1) & 3;
        bool good = false;

        /* left / right within the same row */
        if (col > 0 && curr[col-1] == target) good = true;
        if (col < 1 && curr[col+1] == target) good = true;

        /* up neighbours */
        if (prevExist) {
            if (col > 0 && prev[col-1] == target) good = true;
            if (prev[col] == target) good = true;
            if (col < 1 && prev[col+1] == target) good = true;
        }

        /* down neighbours */
        if (nextExist) {
            if (col > 0 && next[col-1] == target) good = true;
            if (next[col] == target) good = true;
        }

        if (!good) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but keep safe */

    /* Handle the trivial case n = 1 directly */
    if (n == 1) {
        seq_t ans = 0;
        for (int right = 0; right < 4; ++right) {
            int row1 = right << 2;   /* left = 0 */
            if (row_ok(MISSING, row1, MISSING)) {
                ans += 1;
            }
        }
        return ans;
    }

    /* DP arrays: dp[prev][curr] counts of sequences ending with rows prev,curr */
    static seq_t dp[16][16];
    static seq_t next_dp[16][16];

    /* initialise all entries to zero */
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            dp[i][j] = 0;
        }
    }

    /* initial row (row1) has left = 0 */
    for (int right1 = 0; right1 < 4; ++right1) {
        int row1 = right1 << 2;         /* left=0, right=right1 */
        for (int row2 = 0; row2 < 16; ++row2) {
            if (row_ok(MISSING, row1, row2)) {
                dp[row1][row2] += 1;
            }
        }
    }

    /* iterate over remaining rows */
    for (idx_t step = 3; step <= n; ++step) {
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                next_dp[i][j] = 0;

        for (int prev = 0; prev < 16; ++prev) {
            for (int curr = 0; curr < 16; ++curr) {
                seq_t cnt = dp[prev][curr];
                if (cnt == 0) continue;
                for (int next = 0; next < 16; ++next) {
                    if (row_ok(prev, curr, next)) {
                        next_dp[curr][next] += cnt;
                    }
                }
            }
        }
        /* copy back to dp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = next_dp[i][j];
    }

    /* final row check (no further row) */
    seq_t answer = 0;
    for (int prev = 0; prev < 16; ++prev) {
        for (int curr = 0; curr < 16; ++curr) {
            seq_t cnt = dp[prev][curr];
            if (cnt == 0) continue;
            if (row_ok(prev, curr, MISSING)) {
                answer += cnt;
            }
        }
    }
    return answer;
}
