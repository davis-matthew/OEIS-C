/*
 * A298097  Number of nX5 0..1 arrays with every element equal to 3, 4, 5 or 6
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of n×5 0/1 arrays with upper‑left cell = 0 and each cell
 *            having 3–6 king‑move adjacent cells equal to it
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 18, u64 n <= 33, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Sentinel to represent a missing row (topmost or bottommost). */
static const idx_t NONE_ROW = 32u;

/* Directions for king moves (dr,dc).  */
static const int DIRS[8][2] = {
    { -1, -1 }, { -1,  0 }, { -1,  1 },
    {  0, -1 },              {  0,  1 },
    {  1, -1 }, {  1,  0 }, {  1,  1 }
};

/* Pre‑computed transition masks and final‑row validity flags. */
static uint32_t NEXT_MASK[33][32];
static bool     FINAL_OK  [33][32];
static bool     PRECOMPUTED = false;

/* ------------------------------------------------------------------ */
/* Checks whether the middle row 'mid' is valid given the rows
   'above' and 'below'.  NONE_ROW indicates absence of a row.       */
static inline bool
is_valid_row(idx_t above, idx_t mid, idx_t below)
{
    for (int col = 0; col < 5; ++col) {
        int mid_bit = (mid >> col) & 1;
        int same    = 0;

        for (int d = 0; d < 8; ++d) {
            int dr = DIRS[d][0];
            int dc = DIRS[d][1];
            int ncol = col + dc;
            if (ncol < 0 || ncol > 4) continue;

            int nb;
            if (dr == -1) {          /* above row */
                if (above == NONE_ROW) continue;
                nb = (above >> ncol) & 1;
            } else if (dr == 1) {    /* below row */
                if (below == NONE_ROW) continue;
                nb = (below >> ncol) & 1;
            } else {                 /* same row */
                nb = (mid >> ncol) & 1;
            }
            if (nb == mid_bit) ++same;
        }

        if (same < 3 || same > 6) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Build the transition tables once. */
static void
precompute_tables(void)
{
    if (PRECOMPUTED) return;

    for (int above = 0; above <= 32; ++above) {
        for (int cur  = 0; cur < 32; ++cur) {
            uint32_t mask = 0;

            for (int nxt = 0; nxt < 32; ++nxt) {
                if (is_valid_row(above, (idx_t)cur, (idx_t)nxt)) {
                    mask |= (1u << nxt);
                }
            }
            NEXT_MASK[above][cur] = mask;
            FINAL_OK[above][cur] = is_valid_row(above, (idx_t)cur, NONE_ROW);
        }
    }
    PRECOMPUTED = true;
}

/* ------------------------------------------------------------------ */
/* Main sequence function. */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1; not used but harmless */

    precompute_tables();

    /* State array: (above_row, current_row)   */
    const size_t STATE_COUNT = 33 * 32;
    seq_t dp_cur[STATE_COUNT];
    seq_t dp_next[STATE_COUNT];

    /* Init all to zero */
    for (size_t i = 0; i < STATE_COUNT; ++i) dp_cur[i] = 0;

    /* First row: no row above, leftmost cell (bit 0) must be 0. */
    for (int r = 0; r < 32; ++r) {
        if ((r & 1) == 0)                /* leftmost column zero   */
            dp_cur[NONE_ROW * 32 + r] = 1;
    }

    /* Transition over the remaining rows (n-1 steps). */
    for (idx_t step = 1; step < n; ++step) {
        /* Zero next array */
        for (size_t i = 0; i < STATE_COUNT; ++i) dp_next[i] = 0;

        for (int above = 0; above <= 32; ++above) {
            for (int cur = 0; cur < 32; ++cur) {
                seq_t cnt = dp_cur[above * 32 + cur];
                if (!cnt) continue;

                uint32_t mask = NEXT_MASK[above][cur];
                /* Enumerate all allowed next rows */
                for (int nxt = 0; nxt < 32; ++nxt) {
                    if (mask & (1u << nxt)) {
                        dp_next[cur * 32 + nxt] += cnt;
                    }
                }
            }
        }

        /* Swap arrays for next iteration */
        for (size_t i = 0; i < STATE_COUNT; ++i) dp_cur[i] = dp_next[i];
    }

    /* Final row must be valid with no row below. */
    seq_t ans = 0;
    for (int above = 0; above <= 32; ++above) {
        for (int cur = 0; cur < 32; ++cur) {
            seq_t cnt = dp_cur[above * 32 + cur];
            if (cnt && FINAL_OK[above][cur]) {
                ans += cnt;
            }
        }
    }
    return ans;
}
