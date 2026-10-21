/*
 * A241373  Number of 4Xn 0..2 arrays with no element equal to fewer vertical
 *          neighbors than horizontal neighbors, with new values 0..2
 *          introduced in row major order.
 * https://oeis.org/A241373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A241373: Number of 4×n 0..2 arrays with no element equal to fewer vertical
 * neighbors than horizontal neighbors, with new values 0..2 introduced in row-major
 * order.  There is no simple closed form; we compute by dynamic programming
 * over columns.  The algorithm enumerates all 81 possible 4-row columns, filters
 * them according to the 1-before-2 rule, and builds transitions that keep track
 * of whether a 1 has already appeared.  Complexity is O(n·81²), well under 2s
 * for n≈10 000. */

/* Public function required by the test harness */
seq_t a(idx_t n);

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define NUM_COLS 81  /* 3^4 possible columns */

/* ---- Pre‑computed data for each column --------------------------------- */
/* col_val[j][i]  : value in row i (0‑top) of column j */
static int8_t col_val[NUM_COLS][4];
/* vert_same[j][i] : number of vertical neighbors with the same value as cell (i,j) */
static int8_t vert_same[NUM_COLS][4];
/* col_has_one[j] : does column contain a 1? */
static bool col_has_one[NUM_COLS];

/* ---- Transition table: from (left,mid) pair to possible right columns ------ */
struct Trans {
    int cnt;                 /* number of valid right columns */
    int next[NUM_COLS];      /* indices of those columns */
};
static struct Trans trans[NUM_COLS][NUM_COLS];

/* ---- Valid column lists for starting_has_one flag ----------------------- */
static bool valid_flag[2][NUM_COLS];
static int  valid_cols[2][NUM_COLS];
static int  valid_cnt[2];

/* ---- Helper: check whether column `mid` satisfies the vertical/horizontal
 *       constraints when its left and right neighbours are `left` and `right`
 *       (`-1` indicates no neighbour). ----------------------------------- */
static inline bool column_ok(int mid, int left, int right)
{
    for (int i = 0; i < 4; ++i) {
        int v = vert_same[mid][i];
        int h = 0;
        if (left >= 0 && col_val[left][i] == col_val[mid][i]) ++h;
        if (right >= 0 && col_val[right][i] == col_val[mid][i]) ++h;
        if (v < h) return false;
    }
    return true;
}

/* ---- One‑time initialization -------------------------------------------- */
static bool init_done = false;
static void init_once(void)
{
    if (init_done) return;
    init_done = true;

    /* Generate all 81 columns and precompute auxiliary data */
    int idx = 0;
    for (int a = 0; a < 3; ++a)
        for (int b = 0; b < 3; ++b)
            for (int c = 0; c < 3; ++c)
                for (int d = 0; d < 3; ++d) {
                    col_val[idx][0] = a;
                    col_val[idx][1] = b;
                    col_val[idx][2] = c;
                    col_val[idx][3] = d;

                    /* vertical same neighbours */
                    vert_same[idx][0] = (a == b) ? 1 : 0;
                    vert_same[idx][1] = (b == a) ? 1 : 0;
                    vert_same[idx][1] += (b == c) ? 1 : 0;
                    vert_same[idx][2] = (c == b) ? 1 : 0;
                    vert_same[idx][2] += (c == d) ? 1 : 0;
                    vert_same[idx][3] = (d == c) ? 1 : 0;

                    col_has_one[idx] = (a == 1 || b == 1 || c == 1 || d == 1);

                    ++idx;
                }

    /* Build valid column lists for has1 flag 0 and 1 */
    for (int s = 0; s < 2; ++s) {
        int cnt = 0;
        for (int col = 0; col < NUM_COLS; ++col) {
            int local_has_one = s;
            bool ok = true;
            for (int i = 0; i < 4 && ok; ++i) {
                int v = col_val[col][i];
                if (v == 2 && !local_has_one) ok = false;
                if (v == 1) local_has_one = 1;
            }
            if (ok) {
                valid_flag[s][col] = true;
                valid_cols[s][cnt++] = col;
            } else {
                valid_flag[s][col] = false;
            }
        }
        valid_cnt[s] = cnt;
    }

    /* Pre‑compute transition lists for (left,mid) pairs */
    for (int left_col = 0; left_col < NUM_COLS; ++left_col)
        for (int mid_col = 0; mid_col < NUM_COLS; ++mid_col) {
            struct Trans *t = &trans[left_col][mid_col];
            t->cnt = 0;
            for (int r = 0; r < NUM_COLS; ++r) {
                if (column_ok(mid_col, left_col, r)) {
                    t->next[t->cnt++] = r;
                }
            }
        }
}

/* ---- DP arrays: dp[left][mid][h] --------------------------------------- */
static seq_t dp[NUM_COLS][NUM_COLS][2];
static seq_t next_dp[NUM_COLS][NUM_COLS][2];

/* ---- Public function --------------------------------------------------- */
seq_t a(idx_t n)
{
    init_once();

    if (n == 0) return (seq_t)0;   /* offset is 1 – callers use n ≥ 1 */

    /* --- Base case: single column ------------------------------------------------- */
    if (n == 1) {
        seq_t total = 0;
        for (int col = 0; col < NUM_COLS; ++col) {
            if (col_val[col][0] != 0) continue;          /* first cell must be 0 */
            int has_one = 0;
            bool ok = true;
            for (int r = 0; r < 4 && ok; ++r) {
                int v = col_val[col][r];
                if (v == 2 && !has_one) ok = false;
                if (v == 1) has_one = 1;
            }
            if (ok) ++total;
        }
        return total;
    }

    /* --- Initialize DP for the first two columns --------------------------------- */
    memset(dp, 0, sizeof(dp));

    for (int col0 = 0; col0 < NUM_COLS; ++col0) {
        if (col_val[col0][0] != 0) continue;      /* first cell must be 0 */

        /* Validate col0's own 1/2 ordering (no 2 before 1) */
        int has1_after = 0;
        bool ok0 = true;
        for (int r = 0; r < 4 && ok0; ++r) {
            int v = col_val[col0][r];
            if (v == 2 && !has1_after) ok0 = false;
            if (v == 1) has1_after = 1;
        }
        if (!ok0) continue;

        for (int i = 0; i < valid_cnt[has1_after]; ++i) {
            int col1 = valid_cols[has1_after][i];
            if (!column_ok(col0, -1, col1)) continue;
            int new_has1 = has1_after | (col_has_one[col1] ? 1 : 0);
            dp[col0][col1][new_has1] += 1;
        }
    }

    /* --- Process columns 3..n ---------------------------------------------------- */
    for (int col_index = 2; col_index < n; ++col_index) {
        memset(next_dp, 0, sizeof(next_dp));

        for (int left = 0; left < NUM_COLS; ++left) {
            for (int mid = 0; mid < NUM_COLS; ++mid) {
                const struct Trans *t = &trans[left][mid];
                for (int h = 0; h < 2; ++h) {
                    seq_t cnt = dp[left][mid][h];
                    if (!cnt) continue;

                    for (int k = 0; k < t->cnt; ++k) {
                        int newc = t->next[k];
                        /* newc must respect the 1/2 ordering given current h */
                        if (!valid_flag[h][newc]) continue;
                        int nh = h | (col_has_one[newc] ? 1 : 0);
                        next_dp[mid][newc][nh] += cnt;
                    }
                }
            }
        }
        memcpy(dp, next_dp, sizeof(dp));
    }

    /* --- Sum over final states: last column must be valid with its left neighbour
     *     (no right neighbour). -------------------------------------------------- */
    seq_t result = 0;
    for (int left = 0; left < NUM_COLS; ++left) {
        for (int mid = 0; mid < NUM_COLS; ++mid) {
            for (int h = 0; h < 2; ++h) {
                seq_t cnt = dp[left][mid][h];
                if (!cnt) continue;
                if (column_ok(mid, left, -1)) {
                    result += cnt;
                }
            }
        }
    }
    return result;
}
