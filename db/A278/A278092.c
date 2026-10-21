/*
 * A278092  Number of nX6 0..1 arrays with every element both equal and not
 *          equal to some elements at offset (-1,0) (-1,1) (0,-1) (0,1) or
 *          (1,0), with upper left element zero.
 * https://oeis.org/A278092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 binary arrays with every element both equal
 *            and not equal to at least one of its 5 neighbours (-1,0) (-1,1)
 *            (0,-1) (0,1) (1,0), upper‑left element must be 0. Computed by an
 *            automaton DP over the last two rows
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#define COLS 6
#define ROWS (1u << COLS)          /* 64 possible 6‑bit row patterns */
#define DUMMY  ROWS                /* special index for “no previous row” */

/* pre‑computed tables */
static bool init_done = false;
static uint64_t mask_allow[ROWS + 1][ROWS];  /* for each (prev2,prev) mask of allowed cur */
static bool last_ok[ROWS + 1][ROWS];        /* for last row validation */

/* helper to count set bits for all cur patterns that satisfy middle row */
static void precompute_masks(void)
{
    for (int prev2_idx = 0; prev2_idx <= DUMMY; ++prev2_idx) {
        bool has_prev2 = (prev2_idx != DUMMY);
        int prev2_row   = has_prev2 ? prev2_idx : 0;   /* value unused when dummy */

        for (int prev = 0; prev < ROWS; ++prev) {
            uint64_t cur_mask = 0;

            for (int cur = 0; cur < ROWS; ++cur) {
                bool ok = true;
                for (int j = 0; j < COLS; ++j) {
                    int v = (prev >> j) & 1;
                    bool equ = false, diff = false;

                    /* left neighbour */
                    if (j > 0) {
                        int nb = (prev >> (j-1)) & 1;
                        if (nb == v) equ = true; else diff = true;
                    }
                    /* right neighbour */
                    if (j + 1 < COLS) {
                        int nb = (prev >> (j+1)) & 1;
                        if (nb == v) equ = true; else diff = true;
                    }
                    /* up neighbour */
                    if (has_prev2) {
                        int nb = (prev2_row >> j) & 1;
                        if (nb == v) equ = true; else diff = true;
                    }
                    /* up‑right neighbour */
                    if (has_prev2 && j + 1 < COLS) {
                        int nb = (prev2_row >> (j+1)) & 1;
                        if (nb == v) equ = true; else diff = true;
                    }
                    /* down neighbour (cur) */
                    int nb = (cur >> j) & 1;
                    if (nb == v) equ = true; else diff = true;

                    if (!(equ && diff)) { ok = false; break; }
                }
                if (ok) cur_mask |= ((uint64_t)1 << cur);
            }
            mask_allow[prev2_idx][prev] = cur_mask;

            /* last row condition: no down neighbour */
            bool ok_last = true;
            for (int j = 0; j < COLS; ++j) {
                int v = (prev >> j) & 1;
                bool equ = false, diff = false;

                if (j > 0) {
                    int nb = (prev >> (j-1)) & 1;
                    if (nb == v) equ = true; else diff = true;
                }
                if (j + 1 < COLS) {
                    int nb = (prev >> (j+1)) & 1;
                    if (nb == v) equ = true; else diff = true;
                }
                if (has_prev2) {
                    int nb = (prev2_row >> j) & 1;
                    if (nb == v) equ = true; else diff = true;
                }
                if (has_prev2 && j + 1 < COLS) {
                    int nb = (prev2_row >> (j+1)) & 1;
                    if (nb == v) equ = true; else diff = true;
                }
                if (!(equ && diff)) { ok_last = false; break; }
            }
            last_ok[prev2_idx][prev] = ok_last;
        }
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (!init_done) {
        precompute_masks();
        init_done = true;
    }

    if (n == 0) return (seq_t)0;
    if (n == 1) {
        /* only one row: must satisfy last_ok with no up row */
        seq_t res = 0;
        for (int row = 0; row < ROWS; ++row) {
            if ((row & 1) == 0 && last_ok[DUMMY][row]) {
                res += (seq_t)1;
            }
        }
        return res;
    }

    static seq_t dp[ROWS + 1][ROWS];
    static seq_t ndp[ROWS + 1][ROWS];
    /* initialise counts for first row: top‑left cell = 0 */
    memset(dp, 0, sizeof(dp));
    for (int row = 0; row < ROWS; ++row) {
        if ((row & 1) == 0) {      /* column 0 bit must be 0 */
            dp[DUMMY][row] = (seq_t)1;
        }
    }

    /* iterate adding rows 2 … n */
    for (idx_t r = 1; r < n; ++r) {
        memset(ndp, 0, sizeof(ndp));

        for (int prev2 = 0; prev2 <= DUMMY; ++prev2) {
            for (int prev = 0; prev < ROWS; ++prev) {
                seq_t cnt = dp[prev2][prev];
                if (!cnt) continue;
                uint64_t m = mask_allow[prev2][prev];
                while (m) {
                    int cur = __builtin_ctzll(m);
                    ndp[prev][cur] += cnt;
                    m &= m - 1;      /* clear lowest set bit */
                }
            }
        }
        /* swap dp and ndp */
        memcpy(dp, ndp, sizeof(dp));
    }

    /* final row must satisfy last_ok */
    seq_t ans = 0;
    for (int prev2 = 0; prev2 <= DUMMY; ++prev2) {
        for (int prev = 0; prev < ROWS; ++prev) {
            seq_t cnt = dp[prev2][prev];
            if (!cnt) continue;
            if (last_ok[prev2][prev]) ans += cnt;
        }
    }
    return ans;
}
