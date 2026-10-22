/*
 * A300432  Number of nX5 0..1 arrays with every element equal to 1, 2 or 4
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A300432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   this sequence counts the number of n×5 binary arrays with the
 *            following property: – the left‑top element (row 0, col 0) is 0;
 *            – for every cell, the number of equal neighbours among the six
 *            horizontally, vertically or antic‑diagonally adjacent cells must
 *            be 1, 2 or 4
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define COLS 5
#define MASKS (1 << COLS)

static bool precomp_done = false;
static int  trans_cnt[MASKS][MASKS];
static int  trans_next[MASKS][MASKS][MASKS];

/* Check validity of a single cell using the neighbours from the
   previous (`pm`), current (`cm`), and next (`nm`) row masks. */
static bool cell_valid(int col, int val, int pm, int cm, int nm,
                       bool has_pm, bool has_nm,
                       bool has_left, bool has_right)
{
    int cnt = 0;
    /* left/right */
    if (has_left   && ((cm >> (col - 1)) & 1) == val) ++cnt;
    if (has_right  && ((cm >> (col + 1)) & 1) == val) ++cnt;
    /* up/down */
    if (has_pm     && ((pm >> col) & 1) == val) ++cnt;
    if (has_nm     && ((nm >> col) & 1) == val) ++cnt;
    /* anti‑diagonal up-left / down-right */
    if (has_pm && col + 1 < COLS && ((pm >> (col + 1)) & 1) == val) ++cnt;
    if (has_nm && col > 0     && ((nm >> (col - 1)) & 1) == val) ++cnt;

    return (cnt == 1 || cnt == 2 || cnt == 4);
}

/* Check validity of an entire row given its own mask and the
   masks of the neighbouring rows. */
static bool row_is_valid(idx_t row, idx_t nRows,
                         int pm, int cm, int nm)
{
    bool has_pm = row > 0;
    bool has_nm = row + 1 < nRows;
    for (int col = 0; col < COLS; ++col) {
        int val = (cm >> col) & 1;
        bool has_left  = col > 0;
        bool has_right = col + 1 < COLS;
        if (!cell_valid(col, val, pm, cm, nm, has_pm, has_nm,
                        has_left, has_right))
            return false;
    }
    return true;
}

/* Pre‑compute transitions for interior rows (row with both neighbours). */
static void precompute_transitions(void)
{
    if (precomp_done)
        return;
    precomp_done = true;

    /* For a row that has both a previous and a next row. */
    for (int pm = 0; pm < MASKS; ++pm) {
        for (int cm = 0; cm < MASKS; ++cm) {
            int count = 0;
            for (int nm = 0; nm < MASKS; ++nm) {
                if (row_is_valid(1, 3, pm, cm, nm)) {
                    trans_next[pm][cm][count++] = nm;
                }
            }
            trans_cnt[pm][cm] = count;
        }
    }
}

/* Main function requested by the portfolio. */
seq_t a(idx_t n)
{
    /* Special case n == 1 – only the top row exists. */
    if (n == 1) {
        seq_t result = 0;
        for (int m = 0; m < MASKS; ++m) {
            if (m & 1)            /* upper‑left must be 0 */
                continue;
            if (row_is_valid(0, 1, 0, m, 0))
                result += 1;
        }
        return result;
    }

    precompute_transitions();

    /* DP tables: dp[prev][cur] – number of ways up to the current row
       where the last two rows are cur (previous) and prev (current). */
    static seq_t dp[MASKS][MASKS];
    static seq_t ndp[MASKS][MASKS];
    memset(dp, 0, sizeof(dp));

    /* Initialise with the first two rows (row 0 and row 1). */
    for (int m0 = 0; m0 < MASKS; ++m0) {
        if (m0 & 1)            /* upper‑left must be 0 */
            continue;
        for (int m1 = 0; m1 < MASKS; ++m1) {
            if (row_is_valid(0, n, 0, m0, m1))
                dp[m1][m0] += 1;
        }
    }

    /* Process rows 2 … n‑1 (0‑based). */
    for (idx_t r = 2; r < n; ++r) {
        memset(ndp, 0, sizeof(ndp));
        for (int cur = 0; cur < MASKS; ++cur) {      /* second‑last row */
            for (int prev = 0; prev < MASKS; ++prev) {/* last row          */
                seq_t ways = dp[prev][cur];
                if (!ways)
                    continue;
                /* Use transitions for interior rows: pm=cur, cm=prev. */
                int cnt = trans_cnt[cur][prev];
                for (int k = 0; k < cnt; ++k) {
                    int next = trans_next[cur][prev][k];
                    ndp[next][prev] += ways;
                }
            }
        }
        memcpy(dp, ndp, sizeof(dp));
    }

    /* Final row check (row n‑1) – no next row. */
    seq_t answer = 0;
    for (int cur = 0; cur < MASKS; ++cur) {      /* second‑last row */
        for (int prev = 0; prev < MASKS; ++prev) {/* last row       */
            seq_t ways = dp[prev][cur];
            if (!ways)
                continue;
            if (row_is_valid(n - 1, n, cur, prev, 0))
                answer += ways;
        }
    }

    return answer;
}
