/*
 * A301612  Number of nX5 0..1 arrays with every element equal to 1, 2 or 3
 *          horizontally or vertically adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A301612: Count of n×5 0–1 arrays with each element having
 * 1, 2, or 3 adjacent horizontally or vertically equal elements,
 * and the upper‑left element equal to 0.  The implementation uses
 * dynamic programming over rows.  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define ROWS        5
#define PATTERNS    (1 << ROWS)          /* 32 possible row bit patterns */
#define STATE_SIZE  (PATTERNS * PATTERNS)
#define MAX_NEXT    PATTERNS             /* 32 */

/* For each state (prev,row) we store the list of allowed next rows */
static uint8_t trans_next[STATE_SIZE][MAX_NEXT];
static uint8_t trans_count[STATE_SIZE];
static bool trans_inited = false;

/* Check whether a row satisfies the adjacency rule, given its
 * up neighbour (prev) and down neighbour (next).  has_prev/has_next
 * indicate whether the corresponding neighbour exists. */
static bool row_ok(idx_t prev, idx_t curr, idx_t next,
                   bool has_prev, bool has_next)
{
    for (int j = 0; j < ROWS; ++j) {
        int val = (curr >> j) & 1;
        int matches = 0;
        int neighbors = 0;

        if (j > 0) {
            ++neighbors;
            if (((curr >> (j - 1)) & 1) == val) ++matches;
        }
        if (j < ROWS - 1) {
            ++neighbors;
            if (((curr >> (j + 1)) & 1) == val) ++matches;
        }
        if (has_prev) {
            ++neighbors;
            if (((prev >> j) & 1) == val) ++matches;
        }
        if (has_next) {
            ++neighbors;
            if (((next >> j) & 1) == val) ++matches;
        }

        if (matches == 0) return false;            /* 0 equal neighbours forbidden */
        if (neighbors == 4 && matches == 4) return false; /* interior cell can't have 4 equal neighbours */
    }
    return true;
}

/* Pre‑compute the transition tables for interior rows. */
static void init_transitions(void)
{
    for (idx_t prev = 0; prev < PATTERNS; ++prev) {
        for (idx_t curr = 0; curr < PATTERNS; ++curr) {
            idx_t state = prev * PATTERNS + curr;
            uint8_t *list = trans_next[state];
            uint8_t *cnt = &trans_count[state];
            *cnt = 0;
            for (idx_t next = 0; next < PATTERNS; ++next) {
                if (row_ok(prev, curr, next, true, true))
                    list[(*cnt)++] = (uint8_t)next;
            }
        }
    }
    trans_inited = true;
}

/* Count for n==1 directly by brute force. */
static seq_t single_row_count(idx_t)
{
    seq_t total = 0;
    for (idx_t pat = 0; pat < PATTERNS; ++pat) {
        if ((pat & 1) != 0) continue;               /* upper-left fixed 0 */
        if (row_ok(0, pat, 0, false, false))
            total += 1;
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n <= 0) return 0;          /* offset is 1, below that is undefined */

    if (n == 1) return single_row_count(n);

    if (!trans_inited) init_transitions();

    /* DP arrays for counts of states (prev,row). */
    seq_t dp[STATE_SIZE] = {0};
    seq_t ndp[STATE_SIZE] = {0};

    /* Initialisation: first and second rows. */
    for (idx_t top = 0; top < PATTERNS; ++top) {
        if ((top & 1) != 0) continue;         /* upper-left fixed 0 */
        for (idx_t second = 0; second < PATTERNS; ++second) {
            if (row_ok(0, top, second, false, true))
                dp[top * PATTERNS + second] = 1;
        }
    }

    /* Process rows 3 … n. */
    for (idx_t r = 3; r <= n; ++r) {
        /* zero ndp */
        for (size_t i = 0; i < STATE_SIZE; ++i) ndp[i] = 0;

        for (size_t state = 0; state < STATE_SIZE; ++state) {
            if (dp[state] == 0) continue;
            seq_t cur = dp[state];
            idx_t prev = state / PATTERNS;
            idx_t curr = state % PATTERNS;

            for (uint8_t k = 0; k < trans_count[state]; ++k) {
                idx_t next = trans_next[state][k];
                size_t newState = curr * PATTERNS + next;
                ndp[newState] += cur;
            }
        }

        /* swap dp and ndp */
        for (size_t i = 0; i < STATE_SIZE; ++i) dp[i] = ndp[i];
    }

    /* Sum counts where the bottom row satisfies its condition. */
    seq_t result = 0;
    for (size_t state = 0; state < STATE_SIZE; ++state) {
        if (dp[state] == 0) continue;
        idx_t prev = state / PATTERNS;
        idx_t curr = state % PATTERNS;
        if (row_ok(prev, curr, 0, true, false))
            result += dp[state];
    }

    return result;
}
