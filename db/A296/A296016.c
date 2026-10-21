/*
 * A296016  Number of nX5 0..1 arrays with each 1 adjacent to 1 or 4 king-move
 *          neighboring 1s.
 * https://oeis.org/A296016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: computed by row‑by‑row dynamic programming using a transfer
   matrix for 5‑column binary grids where each 1 has 1 or 4 king‑move
   neighbouring 1s. */

#include <stdbool.h>
#include <stdint.h>

#define COLS      5
#define MASK      (1U << COLS)        /* 32 possible rows              */
#define STATES    (MASK * MASK)      /* 1024 (prev,row) state pairs   */

/* Checks that every 1 in 'curr' has exactly 1 or 4 neighbouring 1s
   considering the rows 'prev' and 'next'.  Rows are represented by
   bitmasks of length COLS. */
static inline bool is_valid_row(uint8_t curr, uint8_t prev, uint8_t next)
{
    for (int col = 0; col < COLS; ++col) {
        if (!(curr & (1U << col)))
            continue;                 /* only inspect cells that are 1 */

        int deg = 0;

        /* neighbors in previous row */
        for (int dc = -1; dc <= 1; ++dc) {
            int c = col + dc;
            if (c >= 0 && c < COLS)              /* horizontal wrap? no */
                deg += (prev >> c) & 1U;
        }
        /* neighbors in same row (excluding the cell itself) */
        for (int dc = -1; dc <= 1; ++dc) {
            if (dc == 0) continue;
            int c = col + dc;
            if (c >= 0 && c < COLS)
                deg += (curr >> c) & 1U;
        }
        /* neighbors in next row */
        for (int dc = -1; dc <= 1; ++dc) {
            int c = col + dc;
            if (c >= 0 && c < COLS)
                deg += (next >> c) & 1U;
        }
        if (deg != 1 && deg != 4)
            return false;
    }
    return true;
}

/* Precomputed set of permissible next rows for each pair (prev,curr). */
static uint32_t next_rows[MASK][MASK];
static bool next_rows_ready = false;

/* Public sequence generator */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return 0;                   /* sequence is offset 1 */

    /* Build the transition table once */
    if (!next_rows_ready) {
        for (uint8_t prev = 0; prev < MASK; ++prev) {
            for (uint8_t curr = 0; curr < MASK; ++curr) {
                uint32_t mask = 0;
                for (uint8_t nxt = 0; nxt < MASK; ++nxt)
                    if (is_valid_row(curr, prev, nxt))
                        mask |= 1U << nxt;
                next_rows[prev][curr] = mask;
            }
        }
        next_rows_ready = true;
    }

    seq_t dp[STATES]      = {0};
    seq_t dp_next[STATES] = {0};

    dp[0] = 1;                 /* start with previous=0, current=0 */

    for (idx_t step = 0; step < n; ++step) {
        for (int state = 0; state < STATES; ++state) {
            seq_t cur_val = dp[state];
            if (cur_val == 0)
                continue;

            int prev = state >> COLS;
            int curr = state & (MASK - 1);
            uint32_t mask = next_rows[prev][curr];

            /* Iterate over all next rows whose bits are set in mask */
            for (int nxt = 0; nxt < MASK; ++nxt)
                if (mask & (1U << nxt))
                    dp_next[(curr << COLS) | nxt] += cur_val;
        }

        /* Prepare for next step */
        for (int i = 0; i < STATES; ++i) {
            dp[i] = dp_next[i];
            dp_next[i] = 0;
        }
    }

    /* After all rows processed, the last row is 'curr' with no next row
       (next = 0).  Count only those states where the final row satisfies
       the adjacency rule with next = 0. */
    seq_t result = 0;
    for (int state = 0; state < STATES; ++state) {
        seq_t val = dp[state];
        if (val == 0)
            continue;

        int prev = state >> COLS;
        int curr = state & (MASK - 1);

        if (is_valid_row((uint8_t)curr, (uint8_t)prev, (uint8_t)0))
            result += val;
    }

    return result;
}
