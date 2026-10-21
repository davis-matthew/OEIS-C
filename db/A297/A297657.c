/*
 * A297657  Number of 4 X n 0..1 arrays with every 1 horizontally, diagonally
 *          or antidiagonally adjacent to 1, 2 or 3 neighboring 1's.
 * https://oeis.org/A297657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4×n binary arrays in which every 1 has 1, 2 or
 *            3 neighboring 1's among the horizontally and diagonally adjacent
 *            cells. The algorithm uses a transfer‑matrix DP with a window of
 *            three consecutive columns
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool ok_table[16][16][16];
static bool ok_table_init = false;

static inline bool cell_ok(int prev, int curr, int next, int row) {
    int neigh = 0;
    /* left neighbors */
    if ( (prev >> row) & 1 ) neigh++;
    if (row > 0          && (prev >> (row-1)) & 1 ) neigh++;
    if (row < 3          && (prev >> (row+1)) & 1 ) neigh++;
    /* right neighbors */
    if ( (next >> row) & 1 ) neigh++;
    if (row > 0          && (next >> (row-1)) & 1 ) neigh++;
    if (row < 3          && (next >> (row+1)) & 1 ) neigh++;
    return (neigh >= 1 && neigh <= 3);
}

static inline bool valid_middle(int prev, int curr, int next) {
    for (int r = 0; r < 4; ++r) {
        if ( (curr >> r) & 1 ) {
            if (!cell_ok(prev, curr, next, r))
                return false;
        }
    }
    return true;
}

static void init_ok_table(void) {
    for (int p = 0; p < 16; ++p)
        for (int c = 0; c < 16; ++c)
            for (int n = 0; n < 16; ++n)
                ok_table[p][c][n] = valid_middle(p, c, n);
    ok_table_init = true;
}

seq_t a(idx_t n) {
    if (!ok_table_init)
        init_ok_table();

    seq_t dp[16][16] = {0};
    seq_t ndp[16][16] = {0};

    /* Initial column: left neighbour is all zeros. */
    for (int curr = 0; curr < 16; ++curr)
        dp[0][curr] = 1;

    for (idx_t step = 1; step < n; ++step) {
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                ndp[i][j] = 0;

        for (int prev = 0; prev < 16; ++prev)
            for (int curr = 0; curr < 16; ++curr) {
                seq_t val = dp[prev][curr];
                if (val == 0) continue;
                for (int next = 0; next < 16; ++next) {
                    if (!ok_table[prev][curr][next]) continue;
                    ndp[curr][next] += val;
                }
            }

        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = ndp[i][j];
    }

    seq_t result = 0;
    for (int prev = 0; prev < 16; ++prev)
        for (int curr = 0; curr < 16; ++curr)
            if (ok_table[prev][curr][0])
                result += dp[prev][curr];

    return result;
}
