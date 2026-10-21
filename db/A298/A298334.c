/*
 * A298334  Number of nX5 0..1 arrays with every element equal to 1, 2, 3, 4
 *          or 6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: computes the number of n×5 binary arrays (entries 0 or 1)
 * with the upper‑left element equal to 0 such that every cell has
 * 1, 2, 3, 4, or 6 king‑move adjacent cells equal to it.
 *
 * The algorithm uses dynamic programming over pairs of consecutive rows.
 * A state is (prev_row, curr_row).  The transition to
 * (curr_row, next_row) is allowed iff the current row satisfies
 * the adjacency rule with its above (prev) and below (next) neighbours.
 * The first row has no above neighbours (represented by a special
 * “none” value), and the last row has no below neighbours.
 * After processing all n rows the result is the sum of all
 * terminal states whose last row is valid against a non‑existent
 * bottom row.
 */

#include <stdint.h>
#include <stdbool.h>

#define WIDTH 5                       /* number of columns */
#define ROWS  (1U << WIDTH)           /* number of possible row patterns (32) */
#define NONE  ROWS                     /* special value representing a missing neighbour row */
#define STATE_COUNT ((ROWS + 1) * ROWS) /* number of DP states = 33 × 32 = 1056 */

/* Precomputed adjacency offsets for king moves */
static const int dr[8] = {-1,-1,-1,0,0,1,1,1};
static const int dc[8] = {-1,0,1,-1,1,-1,0,1};

/* Check whether a row satisfies the adjacency rule given its above
   (prev) and below (next) neighbour rows.  If prev or next is NONE,
   those neighbours are omitted. */
static inline bool validRow(int prev, int curr, int next)
{
    for (int j = 0; j < WIDTH; ++j) {
        int currVal = (curr >> j) & 1;
        int cnt = 0;
        for (int k = 0; k < 8; ++k) {
            int cj = j + dc[k];
            if (cj < 0 || cj >= WIDTH) continue;

            int neighbourVal;
            if (dr[k] == -1) {          /* above neighbour */
                if (prev == NONE) continue;
                neighbourVal = (prev >> cj) & 1;
            } else if (dr[k] == 1) {    /* below neighbour */
                if (next == NONE) continue;
                neighbourVal = (next >> cj) & 1;
            } else {                    /* same row neighbour */
                neighbourVal = (curr >> cj) & 1;
            }
            if (neighbourVal == currVal) ++cnt;
        }
        if (cnt != 1 && cnt != 2 && cnt != 3 && cnt != 4 && cnt != 6)
            return false;
    }
    return true;
}

/* Static tables for transitions and initial rows.  They are
   constructed once on the first call. */
static bool initialized = false;
static unsigned char nextCount[STATE_COUNT];
static unsigned char nextList[STATE_COUNT][ROWS];
static unsigned char initRows[ROWS];
static unsigned char initRowCount;

seq_t a(idx_t n)
{
    if (!initialized) {
        /* Precompute transitions for all states. */
        for (int prev = 0; prev <= ROWS; ++prev) {
            for (int curr = 0; curr < ROWS; ++curr) {
                int state = (prev << WIDTH) | curr;
                unsigned char idx = 0;
                for (int next = 0; next < ROWS; ++next) {
                    if (validRow(prev, curr, next)) {
                        nextList[state][idx++] = (unsigned char)next;
                    }
                }
                nextCount[state] = idx;
            }
        }

        /* Build list of possible first‑row patterns (upper‑left cell zero). */
        initRowCount = 0;
        for (int r = 0; r < ROWS; ++r) {
            if ((r & 1) == 0) {  /* bit 0 (leftmost column) is 0 */
                initRows[initRowCount++] = (unsigned char)r;
            }
        }
        initialized = true;
    }

    /* DP arrays for current and next step. */
    seq_t dp[STATE_COUNT];
    seq_t dp_next[STATE_COUNT];
    for (int i = 0; i < STATE_COUNT; ++i) {
        dp[i] = 0;
        dp_next[i] = 0;
    }

    /* Initial state: first row, no row above. */
    for (unsigned char i = 0; i < initRowCount; ++i) {
        int curr = initRows[i];
        int state = (NONE << WIDTH) | curr;
        dp[state] = 1;
    }

    /* Process remaining rows. */
    for (idx_t step = 1; step < n; ++step) {
        /* reset dp_next */
        for (int i = 0; i < STATE_COUNT; ++i) dp_next[i] = 0;

        for (int state = 0; state < STATE_COUNT; ++state) {
            seq_t ways = dp[state];
            if (ways == 0) continue;
            int curr = state & (ROWS - 1);
            unsigned char len = nextCount[state];
            for (unsigned char k = 0; k < len; ++k) {
                int nextRow = nextList[state][k];
                int newState = (curr << WIDTH) | nextRow;
                dp_next[newState] += ways;
            }
        }
        /* swap dp and dp_next */
        for (int i = 0; i < STATE_COUNT; ++i) {
            dp[i] = dp_next[i];
        }
    }

    /* Final check: last row with no row below. */
    seq_t result = 0;
    for (int state = 0; state < STATE_COUNT; ++state) {
        seq_t ways = dp[state];
        if (ways == 0) continue;
        int prev = state >> WIDTH;
        int curr = state & (ROWS - 1);
        if (validRow(prev, curr, NONE)) {
            result += ways;
        }
    }
    return result;
}
