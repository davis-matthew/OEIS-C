/*
 * A278096  Number of n X 3 0..3 arrays with every element plus 1 mod 4 equal
 *          to some element at offset (-1,0) (-1,1) (0,-1) (0,1) or (1,0),
 *          with upper left element zero.
 * https://oeis.org/A278096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computation: a(n) is the number of n×3 arrays with entries in {0,1,2,3}
 * such that for every cell its value+1 (mod 4) appears in one of the
 * neighbour positions (−1,0), (−1,1), (0,−1), (0,1) or (1,0).  The upper‑left
 * entry is fixed to 0.  This implementation uses a transfer‑matrix DP
 * over rows; it runs in O(n·4160) time and produces the exact value
 * for any n ≥ 1 that fits in the supplied seq_t type.
 */

#include <stddef.h>   /* NULL */
#include <stdint.h>   /* uintptr_t etc. */
#include <stdbool.h>  /* bool */
#include <string.h>   /* memset */

#define ROWS 64              /* 4^3 possible rows of length 3 */
#define STATE_CNT (ROWS * (ROWS + 1))  /* (prev,row) pairs, prev may be NONE */

/* Sentinel value indicating “no row” outside the array */
#define NONE 64

/* Pre‑computed digit representation of each row index */
static uint8_t row_digits[ROWS][3];

/* For each state (prev,prevPrev) the list of reachable next states.
   Each list entry encodes the new state (cur,prev).  The maximum
   number of transitions per state is 64. */
static uint16_t next_state[STATE_CNT][64];
static uint8_t next_count[STATE_CNT];

/* ------------------------------------------------------------------ */
/* Helper that checks whether a row satisfies the neighbour condition
   when its upper and lower neighbours are known (NONE means absent). */
static inline bool
row_satisfies(int cur_idx, int above_idx, int below_idx)
{
    const uint8_t *cur  = row_digits[cur_idx];
    const uint8_t *above = (above_idx == NONE) ? NULL : row_digits[above_idx];
    const uint8_t *below = (below_idx == NONE) ? NULL : row_digits[below_idx];

    for (int j = 0; j < 3; ++j) {
        uint8_t val    = cur[j];
        uint8_t target = (uint8_t)((val + 1) & 3);      /* (val+1) mod 4 */
        bool ok = false;
        if (above) {
            if (above[j] == target) ok = true;
            if (j + 1 < 3 && above[j + 1] == target) ok = true;
        }
        if (!ok && j > 0 && cur[j - 1] == target) ok = true;  /* left */
        if (!ok && j + 1 < 3 && cur[j + 1] == target) ok = true;  /* right */
        if (!ok && below && below[j] == target) ok = true;     /* below */
        if (!ok) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Preprocess all rows and build the transition tables.  This function
   is called lazily on the first request for a(n). */
static void
preprocess(void)
{
    /* Encode each row as three base‑4 digits. */
    for (int idx = 0; idx < ROWS; ++idx) {
        row_digits[idx][0] = idx % 4;
        row_digits[idx][1] = (idx / 4) % 4;
        row_digits[idx][2] = (idx / 16) % 4;
    }

    /* Build transition lists. */
    for (int prevPrev = 0; prevPrev <= ROWS; ++prevPrev) {   /* including NONE */
        for (int prev = 0; prev < ROWS; ++prev) {
            int state_idx = prev * (ROWS + 1) + prevPrev;
            uint8_t cnt = 0;
            for (int cur = 0; cur < ROWS; ++cur) {
                if (row_satisfies(prev, prevPrev, cur)) {
                    next_state[state_idx][cnt++] =
                        (uint16_t)(cur * (ROWS + 1) + prev);
                }
            }
            next_count[state_idx] = cnt;
        }
    }
}

/* ------------------------------------------------------------------ */
/* The required OEIS sequence function. */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        preprocess();
        initialized = true;
    }

    /* Undefined region, return 0 safely. */
    if (n == 0) return (seq_t)0;

    /* n == 1 : only one row.  Count rows that satisfy the condition
       when no neighbours exist. */
    if (n == 1) {
        seq_t total = (seq_t)0;
        for (int r = 0; r < ROWS; ++r) {
            if (row_digits[r][0] != 0) continue;          /* upper‑left 0 */
            if (row_satisfies(r, NONE, NONE))
                ++total;
        }
        return total;
    }

    /* DP arrays: DP[state] = number of ways to reach this state after
       processing the current row.  The state is (prev,prevPrev). */
    static seq_t dp[STATE_CNT];
    static seq_t ndp[STATE_CNT];

    /* Initialise dp to zero. */
    memset(dp, 0, sizeof(dp));

    /* Initial state: first row with upper‑left zero, no previous row. */
    for (int r = 0; r < ROWS; ++r) {
        if (row_digits[r][0] != 0) continue;
        int state = r * (ROWS + 1) + NONE;
        dp[state] = (seq_t)1;
    }

    /* Process rows 2 .. n. */
    for (idx_t row = 2; row <= n; ++row) {
        /* Clear the next‑row DP. */
        memset(ndp, 0, sizeof(ndp));

        /* Transition from each existing state to all possible next states. */
        for (int state = 0; state < STATE_CNT; ++state) {
            seq_t val = dp[state];
            if (!val) continue;
            uint8_t cnt = next_count[state];
            uint16_t *list = next_state[state];
            for (uint8_t k = 0; k < cnt; ++k) {
                int nxt = (int)list[k];
                ndp[nxt] += val;
            }
        }

        /* Swap dp and ndp for the next iteration. */
        for (int i = 0; i < STATE_CNT; ++i) dp[i] = ndp[i];
    }

    /* Final check: the last row must satisfy the condition with the
       row above but has no row below.  Sum over all final states. */
    seq_t total = (seq_t)0;
    for (int state = 0; state < STATE_CNT; ++state) {
        seq_t val = dp[state];
        if (!val) continue;
        int prev     = state / (ROWS + 1);
        int prevPrev = state % (ROWS + 1);
        if (prevPrev == NONE) continue;  /* impossible for n>=2 */
        if (row_satisfies(prev, prevPrev, NONE))
            total += val;
    }

    return total;
}
