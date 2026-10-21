/*
 * A252441  Number of (n+2)X(1+2) 0..2 arrays with every 3X3 subblock row,
 *          column, diagonal and antidiagonal sum less than 3.
 * https://oeis.org/A252441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation counts the number a(n) of (n+2)×3 arrays with entries 0,1,2
    such that for every 3×3 sliding subarray all row sums, column sums, main
    diagonal sum and anti‑diagonal sum are ≤ 2.  The rows of the array are
    restricted to those having sum ≤ 2; there are 10 such rows.
    The dynamic programming uses states (i,j) = last two rows of the array.
    Transition: (i,j) → (j,k) is allowed iff rows[i], rows[j], rows[k]
    satisfy the 3×3 constraints.  The DP is initialized with all
    2‑tuples and iterated n times to obtain a(n). */

#include <stdint.h>
#include <stdbool.h>

static const int ROWS[10][3] = {
    {0,0,0}, {1,0,0}, {0,1,0}, {0,0,1},
    {2,0,0}, {0,2,0}, {0,0,2},
    {1,1,0}, {1,0,1}, {0,1,1}
};

extern seq_t a(idx_t n) {
    static bool init = false;
    static bool allowed[10][10][10];
    if (!init) {
        init = true;
        for (int i = 0; i < 10; ++i)
            for (int j = 0; j < 10; ++j)
                for (int k = 0; k < 10; ++k) {
                    bool ok = true;
                    /* row sums */
                    for (int r = 0; r < 3; ++r) {
                        int sum = ROWS[i][r] + ROWS[j][r] + ROWS[k][r];
                        if (sum > 2) { ok = false; break; }
                    }
                    if (!ok) { allowed[i][j][k] = false; continue; }
                    /* column sums */
                    for (int c = 0; c < 3; ++c) {
                        int sum = ROWS[i][c] + ROWS[j][c] + ROWS[k][c];
                        if (sum > 2) { ok = false; break; }
                    }
                    if (!ok) { allowed[i][j][k] = false; continue; }
                    /* main diagonal */
                    int sum = ROWS[i][0] + ROWS[j][1] + ROWS[k][2];
                    if (sum > 2) { allowed[i][j][k] = false; continue; }
                    /* anti-diagonal */
                    sum = ROWS[i][2] + ROWS[j][1] + ROWS[k][0];
                    allowed[i][j][k] = (sum <= 2);
                }
    }

    /* DP arrays: dp[i][j] is number of ways to reach current last two rows (i,j) */
    static seq_t dp[10][10];
    static seq_t dp2[10][10];

    /* initialize with all 2‑tuples */
    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 10; ++j)
            dp[i][j] = (seq_t)1;

    /* iterate to add rows until total size (n+2) is achieved */
    for (idx_t step = 2; step < n + 2; ++step) {
        for (int i = 0; i < 10; ++i)
            for (int j = 0; j < 10; ++j)
                dp2[i][j] = (seq_t)0;

        for (int i = 0; i < 10; ++i)
            for (int j = 0; j < 10; ++j) {
                seq_t add = dp[i][j];
                if (!add) continue;
                for (int k = 0; k < 10; ++k)
                    if (allowed[i][j][k])
                        dp2[j][k] += add;
            }

        /* copy dp2 to dp for next iteration */
        for (int i = 0; i < 10; ++i)
            for (int j = 0; j < 10; ++j)
                dp[i][j] = dp2[i][j];
    }

    /* sum over all final states */
    seq_t ans = (seq_t)0;
    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 10; ++j)
            ans += dp[i][j];

    return ans;
}
