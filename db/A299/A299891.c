/*
 * A299891  Number of nX6 0..1 arrays with every element equal to 0, 2, 3, 4,
 *          5, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A299891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A299891
 * ------------------------------------------------------------
 * Number of n×6 binary arrays (entries are 0 or 1) such that
 * for every cell the number of same‑valued neighbours in the
 * king‑move neighbourhood is in {0,2,3,4,5,7,8} (i.e. not 1 or 6),
 * and the upper‑left cell (row 0, column 0) is 0.
 *
 * The implementation uses a dynamic programming over rows.
 * State: (previous_row, current_row), where each row is a 6‑bit mask.
 * Transition: a new row `next` can follow (prev,cur) iff the row `cur`
 * is valid when its upper neighbour is `prev` (if present) and lower
 * neighbour is `next`.  Only rows that satisfy the condition are
 * enumerated, so the DP is efficient.
 *
 * For n = 1 we count single rows that satisfy the condition with no
 * neighbours above or below.  For n > 1 we initialise the DP with
 * the first row (prev = dummy) and iterate until the nth row.
 * After building the DP the final answer is the sum of all states
 * whose last row is valid with a dummy row below.
 *
 * All constants are defined as macros so that the arrays have
 * compile‑time constant sizes, making the function valid both for
 * C and C++ builds.  The code uses only integer arithmetic and a
 * few logical operations, complying with the requirements of the
 * evaluation harness.
 */

#include <stdbool.h>

/* compile‑time constants ---------------------------------------------------*/
#define WIDTH          6
#define PATTERN_COUNT  (1 << WIDTH)          /* number of possible rows = 64 */
#define DUMMY          PATTERN_COUNT         /* value for a missing neighbour   */
#define STATE_COUNT    ((PATTERN_COUNT + 1) * PATTERN_COUNT)

/* -------------------------------------------------------------------------*/
/* Helper: test whether a row `mid` is valid given its upper and lower neighbours.
   `has_prev` / `has_next` indicate whether the neighbour masks are meaningful.
 */
static inline bool row_ok(int prev, int mid, int next,
                          bool has_prev, bool has_next)
{
    for (int col = 0; col < WIDTH; ++col) {
        int val   = (mid >> col) & 1;
        int cnt   = 0;

        /* same row neighbours */
        if (col > 0   && ((mid >> (col - 1)) & 1) == val) ++cnt;
        if (col < WIDTH-1 && ((mid >> (col + 1)) & 1) == val) ++cnt;

        /* above neighbour row */
        if (has_prev) {
            if (col > 0   && ((prev >> (col - 1)) & 1) == val) ++cnt;
            if (col < WIDTH-1 && ((prev >> (col + 1)) & 1) == val) ++cnt;
            if (((prev >> col) & 1) == val) ++cnt;
        }

        /* below neighbour row */
        if (has_next) {
            if (col > 0   && ((next >> (col - 1)) & 1) == val) ++cnt;
            if (col < WIDTH-1 && ((next >> (col + 1)) & 1) == val) ++cnt;
            if (((next >> col) & 1) == val) ++cnt;
        }

        /* disallowed counts are 1 and 6 */
        if (cnt == 1 || cnt == 6)
            return false;
    }
    return true;
}

/* -------------------------------------------------------------------------*/
/* Core function required by the harness */
seq_t a(idx_t n)
{
    /* Static tables ----------------------------------------------*/
    /*  adjacency lists for transition (prev,cur) -> possible next rows */
    static uint8_t  adj_next[STATE_COUNT][PATTERN_COUNT];
    static uint8_t  adj_len[STATE_COUNT];
    static bool     ready = false;

    /* Precompute adjacency once -----------------------------------*/
    if (!ready) {
        for (int prev = 0; prev <= PATTERN_COUNT; ++prev) {   /* 0..64 (64 is dummy) */
            bool has_prev = (prev != PATTERN_COUNT);
            for (int cur = 0; cur < PATTERN_COUNT; ++cur) {
                int key   = prev * PATTERN_COUNT + cur;
                uint8_t l = 0;
                for (int nxt = 0; nxt < PATTERN_COUNT; ++nxt) {
                    if (row_ok(prev, cur, nxt, has_prev, true)) {
                        adj_next[key][l++] = (uint8_t)nxt;
                    }
                }
                adj_len[key] = l;
            }
        }
        ready = true;
    }

    /* Special case n == 1 ----------------------------------------*/
    if (n == (idx_t)1) {
        seq_t ans = 0;
        for (int row = 0; row < PATTERN_COUNT; ++row) {
            /* upper‑left element must be 0 => bit 0 = 0 */
            if (row & 1) continue;
            if (row_ok(DUMMY, row, DUMMY, false, false))
                ++ans;
        }
        return ans;
    }

    /* DP arrays ---------------------------------------------------*/
    static seq_t dp_cur[STATE_COUNT];
    static seq_t dp_next[STATE_COUNT];

    /* Initialise dp_cur to zero */
    for (int i = 0; i < STATE_COUNT; ++i) dp_cur[i] = 0;

    /* Initialise first row (prev = dummy) */
    int dummy_key_offset = DUMMY * PATTERN_COUNT;   /* index for prev=dummy */
    for (int row = 0; row < PATTERN_COUNT; ++row) {
        if (row & 1) continue;                      /* upper-left must be 0 */
        dp_cur[dummy_key_offset + row] = 1;
    }

    /* DP for rows 2 .. n ----------------------------------------*/
    for (idx_t step = 2; step <= n; ++step) {
        /* Zero dp_next */
        for (int i = 0; i < STATE_COUNT; ++i) dp_next[i] = 0;

        /* Transition */
        for (int idx = 0; idx < STATE_COUNT; ++idx) {
            seq_t val = dp_cur[idx];
            if (val == 0) continue;
            int cur   = idx % PATTERN_COUNT;
            uint8_t l = adj_len[idx];
            for (uint8_t k = 0; k < l; ++k) {
                int nxt = adj_next[idx][k];
                dp_next[cur * PATTERN_COUNT + nxt] += val;
            }
        }

        /* Move dp_next into dp_cur */
        for (int i = 0; i < STATE_COUNT; ++i) dp_cur[i] = dp_next[i];
    }

    /* Final state: last row must be valid with dummy below --------*/
    seq_t ans = 0;
    for (int idx = 0; idx < STATE_COUNT; ++idx) {
        seq_t val = dp_cur[idx];
        if (val == 0) continue;
        int prev = idx / PATTERN_COUNT;
        int cur  = idx % PATTERN_COUNT;
        bool has_prev = (prev != PATTERN_COUNT);
        if (row_ok(prev, cur, DUMMY, has_prev, false))
            ans += val;
    }

    return ans;
}
