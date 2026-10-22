/*
 * A302314  Number of 6Xn 0..1 arrays with every element equal to 0, 1, 3 or 4
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of 6×n 0/1 arrays with each element having 0, 1, 3 or 4
   horizontally, diagonally or antidiagonally adjacent elements,
   upper‑left element fixed to 0.  This is the row 6 of A302309. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/** \brief 6 rows, 64 possible column bitmasks (6 bits).
 *  DUMMY = 64 is used as a sentinel for “no column”. */
#define ROWS 6
#define COLS (1U << ROWS)            /* 64 */
#define DUMMY COLS                   /* 64 */
#define STATE_COUNT ((COLS + 1) * COLS)   /* 65 * 64 = 4160 */

/* Transition validity check for a triple of columns (pprev, prev, next). */
static bool
valid_transition(int pprev, int prev, int next)
{
    for (int r = 0; r < ROWS; ++r) {
        int same = 0;
        int pv   = (prev >> r) & 1;

        /* left side */
        if (pprev != DUMMY) {
            int lp = (pprev >> r) & 1;
            if (lp == pv) ++same;
            if (r > 0   && ((pprev >> (r - 1)) & 1) == pv) ++same;
            if (r < ROWS-1 && ((pprev >> (r + 1)) & 1) == pv) ++same;
        }

        /* right side */
        int rp = (next >> r) & 1;
        if (rp == pv) ++same;
        if (r > 0   && ((next >> (r - 1)) & 1) == pv) ++same;
        if (r < ROWS-1 && ((next >> (r + 1)) & 1) == pv) ++same;

        /* allowed counts are 0,1,3,4 */
        if (same == 2 || same == 5 || same == 6) return false;
    }
    return true;
}

/* Validity check when there is no column on the right side. */
static bool
valid_last(int pprev, int prev)
{
    for (int r = 0; r < ROWS; ++r) {
        int same = 0;
        int pv   = (prev >> r) & 1;

        if (pprev != DUMMY) {
            int lp = (pprev >> r) & 1;
            if (lp == pv) ++same;
            if (r > 0   && ((pprev >> (r - 1)) & 1) == pv) ++same;
            if (r < ROWS-1 && ((pprev >> (r + 1)) & 1) == pv) ++same;
        }

        if (same == 2 || same == 5 || same == 6) return false;
    }
    return true;
}

/* Pre‑computed bitmasks: for every state (pprev,prev) a 64‑bit mask
 * of all next columns that keep 'prev' valid. */
static uint64_t transition_mask[STATE_COUNT];
static bool     mask_prepared = false;

static void
prepare_masks(void)
{
    if (mask_prepared) return;
    for (int pprev = 0; pprev <= COLS; ++pprev) {           /* DUMMY inclusive */
        for (int prev = 0; prev < COLS; ++prev) {
            uint64_t m = 0;
            for (int nxt = 0; nxt < COLS; ++nxt) {
                if (valid_transition(pprev, prev, nxt))
                    m |= (uint64_t)1 << nxt;
            }
            transition_mask[pprev * COLS + prev] = m;
        }
    }
    mask_prepared = true;
}

/** Return a(n) for n ≥ 1, with offset 1 (a(1) is the first listed
 *  term).  For n = 0 the function returns 0. */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;                       /* offset not used */

    prepare_masks();

    static seq_t dp[STATE_COUNT];
    static seq_t dp2[STATE_COUNT];
    for (size_t i = 0; i < STATE_COUNT; ++i) dp[i] = 0;

    /* Initialize column 1: only bit 0 (top-left) forced to 0. */
    for (int col1 = 0; col1 < COLS; ++col1) {
        if (((col1 >> 0) & 1) != 0) continue;   /* top‑left must be 0 */
        dp[DUMMY * COLS + col1] = 1;
    }

    /* Extend columns 2 … n. */
    for (idx_t col = 2; col <= n; ++col) {
        for (size_t i = 0; i < STATE_COUNT; ++i) dp2[i] = 0;

        for (size_t s = 0; s < STATE_COUNT; ++s) {
            seq_t ways = dp[s];
            if (ways == 0) continue;

            int pprev = (int)(s / COLS);
            int prev  = (int)(s % COLS);
            uint64_t bits = transition_mask[s];

            while (bits) {
                unsigned nxt = __builtin_ctzll(bits);
                bits &= bits - 1;                 /* clear lowest set bit */
                size_t new_state = (size_t)prev * COLS + nxt;
                dp2[new_state] = dp2[new_state] + ways;
            }
        }
        for (size_t i = 0; i < STATE_COUNT; ++i) dp[i] = dp2[i];
    }

    /* Final validation of the last column (no right neighbour). */
    seq_t ans = 0;
    for (size_t s = 0; s < STATE_COUNT; ++s) {
        seq_t ways = dp[s];
        if (ways == 0) continue;

        int pprev = (int)(s / COLS);
        int prev  = (int)(s % COLS);
        if (valid_last(pprev, prev))
            ans = ans + ways;
    }
    return ans;
}
