/*
 * A303686  Number of nX4 0..1 arrays with every element unequal to 2, 3 or 4
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A303686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of n×4 0..1 arrays with the king‑move adjacency
 *            inequality constraint, upper‑left element forced to 0.
 *            Computation via dynamic programming over rows using pre‑computed
 *            validity of triples of rows
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define NULL_ROW 16     /* index of the “no row” sentinel */
#define MAX_MASK 16     /* 4‑bit masks 0..15 */

/* Pre‑computed table indicating whether a triple (up,row,down) satisfies
   the inequality constraint for the middle row, where up or down may
   be the sentinel NULL_ROW meaning “no neighbor above/below”. */
static unsigned char row_ok[MAX_MASK + 1][MAX_MASK][MAX_MASK + 1];

/* Initialise the pre‑computation table.  Must be called before any
   calls to a() */
static void init_ok(void) {
    unsigned int up, mid, down;
    for (up = 0; up <= MAX_MASK; ++up) {
        int has_up = (up != NULL_ROW);
        for (mid = 0; mid < MAX_MASK; ++mid) {
            unsigned int mid_bits = mid;
            for (down = 0; down <= MAX_MASK; ++down) {
                int has_down = (down != NULL_ROW);
                int valid = 1;
                /* check all 4 columns of the middle row */
                for (int j = 0; j < 4; ++j) {
                    int cur = (mid_bits >> j) & 1;
                    int cnt = 0;
                    /* Left */
                    if (j > 0) {
                        int n = (mid_bits >> (j - 1)) & 1;
                        if (n != cur) ++cnt;
                    }
                    /* Right */
                    if (j < 3) {
                        int n = (mid_bits >> (j + 1)) & 1;
                        if (n != cur) ++cnt;
                    }
                    /* Up */
                    if (has_up) {
                        int n = (up >> j) & 1;
                        if (n != cur) ++cnt;
                    }
                    /* Down */
                    if (has_down) {
                        int n = (down >> j) & 1;
                        if (n != cur) ++cnt;
                    }
                    /* Up‑left */
                    if (has_up && j > 0) {
                        int n = (up >> (j - 1)) & 1;
                        if (n != cur) ++cnt;
                    }
                    /* Up‑right */
                    if (has_up && j < 3) {
                        int n = (up >> (j + 1)) & 1;
                        if (n != cur) ++cnt;
                    }
                    /* Down‑left */
                    if (has_down && j > 0) {
                        int n = (down >> (j - 1)) & 1;
                        if (n != cur) ++cnt;
                    }
                    /* Down‑right */
                    if (has_down && j < 3) {
                        int n = (down >> (j + 1)) & 1;
                        if (n != cur) ++cnt;
                    }
                    if (cnt < 2 || cnt > 4) {
                        valid = 0;
                        break;
                    }
                }
                row_ok[up][mid][down] = valid;
            }
        }
    }
}

/* The main sequence function. */
seq_t a(idx_t n) {
    /* Initialise pre‑computation once (thread‑unsafe but fine for single‑threaded) */
    static int did_init = 0;
    if (!did_init) {
        init_ok();
        did_init = 1;
    }

    /* DP arrays: dp[prev_up][prev]  (prev_up may be NULL_ROW) */
    seq_t dp[MAX_MASK + 1][MAX_MASK] = {0};
    seq_t ndp[MAX_MASK + 1][MAX_MASK] = {0};

    /* Initial row (row 1) masks: upper‑left cell (col 0) must be 0 */
    for (int mask = 0; mask < MAX_MASK; ++mask) {
        if ((mask & 1) == 0) { /* lower bit is column 0 */
            dp[NULL_ROW][mask] = 1;
        }
    }

    /* Special case n == 1 : check the single row constraints with no neighbors */
    if (n == 1) {
        seq_t ans = 0;
        for (int mask = 0; mask < MAX_MASK; ++mask) {
            if ((mask & 1) == 0 && row_ok[NULL_ROW][mask][NULL_ROW]) {
                ans += dp[NULL_ROW][mask];
            }
        }
        return ans;
    }

    /* Process rows 2 … n */
    for (idx_t row = 2; row <= n; ++row) {
        /* zero ndp */
        for (int up = 0; up <= MAX_MASK; ++up)
            for (int mid = 0; mid < MAX_MASK; ++mid)
                ndp[up][mid] = 0;

        for (int up = 0; up <= MAX_MASK; ++up) {
            for (int mid = 0; mid < MAX_MASK; ++mid) {
                seq_t cur_val = dp[up][mid];
                if (!cur_val) continue;
                for (int down = 0; down < MAX_MASK; ++down) {
                    if (!row_ok[up][mid][down]) continue;
                    ndp[mid][down] += cur_val;
                }
            }
        }
        /* swap dp and ndp */
        for (int up = 0; up <= MAX_MASK; ++up)
            for (int mid = 0; mid < MAX_MASK; ++mid)
                dp[up][mid] = ndp[up][mid];
    }

    /* After all rows processed, dp[up][mid] holds counts with up=row n-1,
       mid=row n, and constraints for rows 1..n-1 already satisfied. */
    seq_t ans = 0;
    for (int up = 0; up < MAX_MASK; ++up) {
        for (int mid = 0; mid < MAX_MASK; ++mid) {
            if (!dp[up][mid]) continue;
            if (row_ok[up][mid][NULL_ROW]) {
                ans += dp[up][mid];
            }
        }
    }
    return ans;
}
