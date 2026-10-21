/*
 * A295844  Number of nX5 0..1 arrays with each 1 adjacent to 1 or 2 king-move
 *          neighboring 1s.
 * https://oeis.org/A295844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A295844: Number of n×5 binary arrays in which every 1 has
    exactly 1 or 2 king‑move neighbors among the other 1’s.

    The sequence is computed by a transfer‑matrix dynamic‑programming
    algorithm.  States are 5‑bit row patterns together with a 10‑bit
    *remaining* vector that records, for each 1 in the current row,
    how many neighbours it still needs from the next row (0, 1 or 2).
    Transitions enforce that all required neighbours are supplied by
    the next row and also generate the appropriate remaining vector
    for that row.  After the last row, all remaining counts must be 0.
    The answer for a given n is the sum of all diagonal states
    (remaining vector = 0). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* Constants for the problem. */
#define COLS 5
#define MASKS (1 << COLS)          /* 32 */
#define MAX_REM (1 << (2 * COLS))  /* 4^5 = 1024 possible remaining vectors */

/* Precomputed tables. */
static int horiz_deg[MASKS][COLS];                  /* horizontal neighbours within a row */
static int e_cur[MASKS][MASKS][COLS];              /* edges from row A to row B (for each A vertex) */
static int e_next[MASKS][MASKS][COLS];             /* edges from row A to row B (for each B vertex) */
static int total_deg[MASKS][MASKS][COLS];          /* horiz_deg(B)[j] + e_next[A][B][j]  */

/* Recursive helper for generating remaining vectors for the next row. */
static void dfs_next(int pos, int next_mask, const int *degs,
                     int code, seq_t val, seq_t (*next)[MAX_REM])
{
    if (pos == COLS) {
        next[next_mask][code] += val;
        return;
    }
    if (!(next_mask & (1 << pos))) {
        dfs_next(pos + 1, next_mask, degs, code, val, next);
        return;
    }
    int curDeg = degs[pos];
    if (curDeg > 2) return;          /* impossible, prune */
    if (curDeg == 2) {
        dfs_next(pos + 1, next_mask, degs, code | (0 << (2 * pos)), val, next);
    } else if (curDeg == 1) {
        dfs_next(pos + 1, next_mask, degs, code | (0 << (2 * pos)), val, next);
        dfs_next(pos + 1, next_mask, degs, code | (1 << (2 * pos)), val, next);
    } else { /* curDeg == 0 */
        dfs_next(pos + 1, next_mask, degs, code | (1 << (2 * pos)), val, next);
        dfs_next(pos + 1, next_mask, degs, code | (2 << (2 * pos)), val, next);
    }
}

/* Recursive helper for generating initial remaining vectors for the first row. */
static void dfs_initial(int pos, int mask, const int *degs,
                        int code, seq_t (*dp)[MAX_REM])
{
    if (pos == COLS) {
        dp[mask][code] += 1;
        return;
    }
    if (!(mask & (1 << pos))) {
        dfs_initial(pos + 1, mask, degs, code, dp);
        return;
    }
    int curDeg = degs[pos];
    if (curDeg > 2) return;
    if (curDeg == 2) {
        dfs_initial(pos + 1, mask, degs, code | (0 << (2 * pos)), dp);
    } else if (curDeg == 1) {
        dfs_initial(pos + 1, mask, degs, code | (0 << (2 * pos)), dp);
        dfs_initial(pos + 1, mask, degs, code | (1 << (2 * pos)), dp);
    } else { /* curDeg == 0 */
        dfs_initial(pos + 1, mask, degs, code | (1 << (2 * pos)), dp);
        dfs_initial(pos + 1, mask, degs, code | (2 << (2 * pos)), dp);
    }
}

/* Main function implementing the sequence. */
seq_t a(idx_t n)
{
    /* Precompute tables once. */
    static bool tables_ready = false;
    if (!tables_ready) {
        for (int mask = 0; mask < MASKS; ++mask) {
            for (int j = 0; j < COLS; ++j) {
                int left  = (mask & (1 << (j - 1))) ? 1 : 0;
                int right = (mask & (1 << (j + 1))) ? 1 : 0;
                horiz_deg[mask][j] = left + right;
            }
        }
        for (int a = 0; a < MASKS; ++a) {
            for (int b = 0; b < MASKS; ++b) {
                for (int i = 0; i < COLS; ++i) e_cur[a][b][i] = 0;
                for (int j = 0; j < COLS; ++j) e_next[a][b][j] = 0;
                for (int i = 0; i < COLS; ++i) {
                    if (!(a & (1 << i))) continue;
                    for (int j = 0; j < COLS; ++j) {
                        if (!(b & (1 << j))) continue;
                        if (i > j + 1 || i < j - 1) continue;
                        e_cur[a][b][i] += 1;
                        e_next[a][b][j] += 1;
                    }
                }
                for (int j = 0; j < COLS; ++j) {
                    total_deg[a][b][j] = horiz_deg[b][j] + e_next[a][b][j];
                }
            }
        }
        tables_ready = true;
    }

    /* DP arrays. */
    static seq_t dp[MASKS][MAX_REM];
    static seq_t next_dp[MASKS][MAX_REM];
    /* initialize to zero */
    for (int i = 0; i < MASKS; ++i)
        for (int j = 0; j < MAX_REM; ++j)
            dp[i][j] = 0;

    /* first row initialization */
    for (int mask = 0; mask < MASKS; ++mask) {
        dfs_initial(0, mask, horiz_deg[mask], 0, dp);
    }

    /* iterate over remaining rows */
    for (idx_t row = 1; row < n; ++row) {
        /* zero next_dp */
        for (int i = 0; i < MASKS; ++i)
            for (int j = 0; j < MAX_REM; ++j)
                next_dp[i][j] = 0;

        for (int cur_mask = 0; cur_mask < MASKS; ++cur_mask) {
            for (int code = 0; code < MAX_REM; ++code) {
                seq_t val = dp[cur_mask][code];
                if (!val) continue;

                for (int next_mask = 0; next_mask < MASKS; ++next_mask) {
                    bool ok = true;
                    /* Check that remaining counts of current row are satisfied. */
                    for (int i = 0; i < COLS; ++i) {
                        if (!(cur_mask & (1 << i))) continue;
                        int rem = (code >> (2 * i)) & 3;
                        if (rem != e_cur[cur_mask][next_mask][i]) {
                            ok = false; break;
                        }
                    }
                    if (!ok) continue;

                    /* Build degree array for next row. */
                    int degs[COLS];
                    for (int j = 0; j < COLS; ++j)
                        degs[j] = total_deg[cur_mask][next_mask][j];

                    /* Generate all possible remaining vectors for the next row. */
                    dfs_next(0, next_mask, degs, 0, val, next_dp);
                }
            }
        }
        /* swap dp and next_dp */
        for (int i = 0; i < MASKS; ++i)
            for (int j = 0; j < MAX_REM; ++j)
                dp[i][j] = next_dp[i][j];
    }

    /* final answer: sum of states with remaining = 0 (code == 0). */
    seq_t ans = 0;
    for (int mask = 0; mask < MASKS; ++mask)
        ans += dp[mask][0];
    return ans;
}
