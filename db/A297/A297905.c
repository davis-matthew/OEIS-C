/*
 * A297905  Number of nX6 0..1 arrays with every element equal to 0, 1, 3 or 4
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A297905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 16, u64 n <= 31, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: number of n×6 binary arrays with each element having
   exactly 0, 1, 3 or 4 king‑move adjacent elements of the same value,
   with the upper‑left element fixed to 0.  The algorithm performs a
   linear DP over the rows; states are pairs of successive rows
   (previous, current).  All transitions are precomputed once. */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define MAX_COLS 6
#define MAX_ROWS (1 << MAX_COLS)           /* 64 possible rows   */
#define STATE_COUNT (MAX_ROWS * MAX_ROWS)   /* 4096 (prev,row) pairs */
#define MAX_TRANS  MAX_ROWS                 /* at most 64 transitions per state */

/* transition tables and start states (computed once) */
static int transition_len[STATE_COUNT];
static int transition_next[STATE_COUNT][MAX_TRANS];
static int start_states[STATE_COUNT];
static int start_len = 0;
static bool precomputed = false;

/* Test whether a single row satisfies the neighbor‑count condition
   given the above and below rows (-1 means nonexistent).  */
static inline bool valid_row(int above, int cur, int below)
{
    for (int j = 0; j < MAX_COLS; ++j) {
        int cell = (cur >> j) & 1;
        int cnt = 0;

        /* same row */
        if (j > 0   && ((cur >> (j - 1)) & 1) == cell) ++cnt;
        if (j < MAX_COLS - 1 && ((cur >> (j + 1)) & 1) == cell) ++cnt;

        /* above row */
        if (above >= 0) {
            if (j > 0   && ((above >> (j - 1)) & 1) == cell) ++cnt;
            if (((above >> j) & 1) == cell) ++cnt;
            if (j < MAX_COLS - 1 && ((above >> (j + 1)) & 1) == cell) ++cnt;
        }

        /* below row */
        if (below >= 0) {
            if (j > 0   && ((below >> (j - 1)) & 1) == cell) ++cnt;
            if (((below >> j) & 1) == cell) ++cnt;
            if (j < MAX_COLS - 1 && ((below >> (j + 1)) & 1) == cell) ++cnt;
        }

        /* allowed neighbor counts are 0, 1, 3, or 4 */
        if (cnt != 0 && cnt != 1 && cnt != 3 && cnt != 4) return false;
    }
    return true;
}

/* Precompute transition tables and initial states (once). */
static void precompute(void)
{
    if (precomputed) return;

    for (int prev = 0; prev < MAX_ROWS; ++prev) {
        for (int cur = 0; cur < MAX_ROWS; ++cur) {
            int idx = prev * MAX_ROWS + cur;
            int tlen = 0;
            for (int next = 0; next < MAX_ROWS; ++next) {
                if (valid_row(prev, cur, next)) {
                    transition_next[idx][tlen++] = next;
                }
            }
            transition_len[idx] = tlen;
        }
    }

    /* start states: first two rows, upper‑left zero, first row valid with second */
    start_len = 0;
    for (int r1 = 0; r1 < MAX_ROWS; ++r1) {
        if (r1 & 1) continue;                 /* upper‑left must be 0 */
        for (int r2 = 0; r2 < MAX_ROWS; ++r2) {
            if (valid_row(-1, r1, r2)) {
                start_states[start_len++] = r1 * MAX_ROWS + r2;
            }
        }
    }

    precomputed = true;
}

/* Main sequence function requested by the exercise */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined; safe return */

    /* n == 1: single row matrix */
    if (n == 1) {
        seq_t cnt = 0;
        for (int row = 0; row < MAX_ROWS; ++row) {
            if (row & 1) continue;              /* upper‑left zero */
            if (valid_row(-1, row, -1)) cnt += 1;
        }
        return cnt;
    }

    precompute();

    /* DP arrays for counts and active state bookkeeping */
    static seq_t dp_cur[STATE_COUNT];
    static seq_t dp_next[STATE_COUNT];
    static int active[STATE_COUNT];
    static int next_active[STATE_COUNT];
    int active_cnt = 0, next_cnt = 0;

    /* initialise dp_cur with start states */
    memset(dp_cur, 0, sizeof(dp_cur));
    for (int i = 0; i < start_len; ++i) {
        int idx = start_states[i];
        dp_cur[idx] = 1;
        active[active_cnt++] = idx;
    }

    /* iterate rows 3 .. n */
    for (idx_t step = 2; step < n; ++step) {
        /* clear next array and visited flags */
        memset(dp_next, 0, sizeof(dp_next));
        memset(next_active, 0, sizeof(next_active));
        next_cnt = 0;

        bool visited[STATE_COUNT];
        memset(visited, 0, sizeof(visited));

        /* process current active states */
        for (int ai = 0; ai < active_cnt; ++ai) {
            int idx = active[ai];
            seq_t ways = dp_cur[idx];
            if (ways == 0) continue;
            int cur = idx % MAX_ROWS;
            int tlen = transition_len[idx];
            for (int t = 0; t < tlen; ++t) {
                int nextRow = transition_next[idx][t];
                int new_idx = cur * MAX_ROWS + nextRow;
                dp_next[new_idx] += ways;
                if (!visited[new_idx]) {
                    visited[new_idx] = true;
                    next_active[next_cnt++] = new_idx;
                }
            }
        }

        /* swap old and new DP arrays */
        memcpy(dp_cur, dp_next, sizeof(dp_cur));

        /* prepare next active list */
        active_cnt = next_cnt;
        memcpy(active, next_active, active_cnt * sizeof(int));
    }

    /* final sum: last row must be valid with no below row */
    seq_t ans = 0;
    for (int ai = 0; ai < active_cnt; ++ai) {
        int idx = active[ai];
        int prev = idx / MAX_ROWS;
        int cur  = idx % MAX_ROWS;
        if (valid_row(prev, cur, -1)) {
            ans += dp_cur[idx];
        }
    }

    return ans;
}
