/*
 * A303633  Number of nX5 0..1 arrays with every element unequal to 2 or 3
 *          horizontally or vertically adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A303633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 38, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: Dynamic programming over the rows of a 5‑column binary
   matrix.  Each cell must differ from exactly 2 or 3 of its horizontal
   or vertical neighbours. The upper‑left cell (row 1, column 1) is fixed
   to 0.  The function returns a(n) for n ≥ 1. */

#include <stdbool.h>

static inline bool row_ok(int prev, int cur, int next,
                          bool up_present, bool down_present)
{
    for (int j = 0; j < 5; ++j) {
        int val = (cur >> j) & 1;
        int diff = 0;
        if (j > 0 && (((cur >> (j - 1)) & 1) != val)) diff++;
        if (j < 4 && (((cur >> (j + 1)) & 1) != val)) diff++;
        if (up_present   && (((prev >> j) & 1) != val)) diff++;
        if (down_present && (((next >> j) & 1) != val)) diff++;
        if (diff != 2 && diff != 3) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* a(1) = 0, no 1×5 array satisfies the adjacency rule with
       the upper‑left cell fixed to 0. */
    if (n <= 1) return 0;

    /* Pre‑compute admissibility tables. */
    bool row1_ok[32][32];
    bool last_ok[32][32];
    bool mid_ok[32][32][32];

    /* Initialise tables to false. */
    for (int r1 = 0; r1 < 32; ++r1)
        for (int r2 = 0; r2 < 32; ++r2)
            row1_ok[r1][r2] = false;

    for (int up = 0; up < 32; ++up)
        for (int cur = 0; cur < 32; ++cur)
            last_ok[up][cur] = false;

    for (int up = 0; up < 32; ++up)
        for (int cur = 0; cur < 32; ++cur)
            for (int down = 0; down < 32; ++down)
                mid_ok[up][cur][down] = false;

    /* Row 1 has no upper neighbour but has a lower neighbour. */
    for (int r1 = 0; r1 < 32; ++r1) {
        /* upper‑left cell must be 0 (bit 0 = 0). */
        if (r1 & 1) continue;        /* skip rows with a 1 in the top‑left cell */
        for (int r2 = 0; r2 < 32; ++r2) {
            if (row_ok(0, r1, r2, false, true))
                row1_ok[r1][r2] = true;
        }
    }

    /* Last row has no lower neighbour but has an upper neighbour. */
    for (int up = 0; up < 32; ++up)
        for (int cur = 0; cur < 32; ++cur)
            if (row_ok(up, cur, 0, true, false))
                last_ok[up][cur] = true;

    /* Middle rows have both upper and lower neighbours. */
    for (int up = 0; up < 32; ++up)
        for (int cur = 0; cur < 32; ++cur)
            for (int down = 0; down < 32; ++down)
                if (row_ok(up, cur, down, true, true))
                    mid_ok[up][cur][down] = true;

    /* Dynamic programming: dp[prev][curr] = number of ways to reach
       state (prev, curr) after processing the current row. */
    static seq_t dp[32][32];
    static seq_t next[32][32];

    /* Initialise dp to zero. */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = 0;

    /* Initialise with the first two rows satisfying the top‑row rules. */
    for (int r1 = 0; r1 < 32; ++r1) {
        for (int r2 = 0; r2 < 32; ++r2) {
            if (row1_ok[r1][r2])
                dp[r1][r2] += 1;
        }
    }

    /* Process rows 3 … n. */
    for (idx_t r = 3; r <= n; ++r) {
        /* Zero the next array. */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                next[i][j] = 0;

        /* Transition from state (prev, curr) to (curr, nxt). */
        for (int prev = 0; prev < 32; ++prev) {
            for (int curr = 0; curr < 32; ++curr) {
                seq_t cnt = dp[prev][curr];
                if (!cnt) continue;
                for (int nxt = 0; nxt < 32; ++nxt) {
                    if (mid_ok[prev][curr][nxt])
                        next[curr][nxt] += cnt;
                }
            }
        }

        /* Copy next back to dp. */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = next[i][j];
    }

    /* Final step: last row must satisfy its constraints. */
    seq_t ans = 0;
    for (int prev = 0; prev < 32; ++prev) {
        for (int curr = 0; curr < 32; ++curr) {
            if (last_ok[prev][curr])
                ans += dp[prev][curr];
        }
    }

    return ans;
}
