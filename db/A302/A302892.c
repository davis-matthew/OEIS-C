/*
 * A302892  Number of 5 X n 0..1 arrays with every element equal to 0, 2, 3, 4
 *          or 5 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A302892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 34,
 *            big any n
 * Helpers:   _valid_neighbors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Problem: counts 5×n binary arrays with upper‑left cell fixed to 0.
   For every cell, the number of equal neighbours among its horizontal,
   diagonal and anti‑diagonal adjacencies must be 0, 2, 3, 4 or 5.
   No simple closed form is known; the function uses dynamic programming
   over the 32 possible 5‑bit column patterns. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/_valid_neighbors.h"

static bool _tables_ready = false;

/* Tables for transitions between 5‑bit column patterns. */
static bool _allowed_full[32][32][32];
static bool _allowed_left[32][32];
static bool _allowed_right[32][32];
static uint8_t _next_list[32][32][32];
static uint8_t _next_count[32][32];

/* Pre‑compute tables once. */
static void _init_tables(void)
{
    if (_tables_ready)
        return;

    memset(_allowed_full, 0, sizeof(_allowed_full));
    memset(_allowed_left, 0, sizeof(_allowed_left));
    memset(_allowed_right, 0, sizeof(_allowed_right));
    memset(_next_count, 0, sizeof(_next_count));

    /* Compute complete transitions and right‑boundary table. */
    for (uint32_t prev = 0; prev < 32; ++prev) {
        for (uint32_t curr = 0; curr < 32; ++curr) {
            if (_valid_neighbors(prev, curr, 0, false, true))
                _allowed_right[prev][curr] = true;

            for (uint32_t next = 0; next < 32; ++next) {
                if (_valid_neighbors(prev, curr, next, false, false)) {
                    _allowed_full[prev][curr][next] = true;
                    _next_list[prev][curr][_next_count[prev][curr]++] =
                        (uint8_t)next;
                }
            }
        }
    }

    /* Compute left‑boundary table. */
    for (uint32_t curr = 0; curr < 32; ++curr) {
        for (uint32_t next = 0; next < 32; ++next) {
            if (_valid_neighbors(0, curr, next, true, false))
                _allowed_left[curr][next] = true;
        }
    }
    _tables_ready = true;
}

seq_t a(idx_t n)
{
    _init_tables();

    /* First column must have upper‑left bit 0.  There are 2^4 = 16
       free choices for the other rows. */
    const uint32_t top_bit_mask = 0x1;          /* bit 0 = top row */
    const uint32_t first_column_count = 16;     /* 2^4 */

    if (n == 1) {
        return (seq_t)first_column_count;
    }

    /* DP arrays: dp[prev][curr] holds number of ways for the last two columns. */
    static seq_t dp[32][32];
    static seq_t ndp[32][32];
    memset(dp, 0, sizeof(dp));

    /* Initialise from the left boundary: only need to consider the first two columns. */
    for (uint32_t col0 = 0; col0 < 32; ++col0) {
        if (col0 & top_bit_mask)            /* top cell must be 0 */
            continue;
        for (uint32_t col1 = 0; col1 < 32; ++col1) {
            if (_allowed_left[col0][col1])   /* left‑boundary condition for col0 */
                dp[col0][col1] += (seq_t)1;
        }
    }

    /* Expand to the right for the remaining columns. */
    for (idx_t step = 0; step < n - 2; ++step) {
        memset(ndp, 0, sizeof(ndp));
        for (uint32_t prev = 0; prev < 32; ++prev) {
            for (uint32_t curr = 0; curr < 32; ++curr) {
                seq_t val = dp[prev][curr];
                if (val == 0)
                    continue;
                uint8_t cnt = _next_count[prev][curr];
                for (uint8_t k = 0; k < cnt; ++k) {
                    uint32_t next = _next_list[prev][curr][k];
                    ndp[curr][next] += val;
                }
            }
        }
        memcpy(dp, ndp, sizeof(dp));
    }

    /* Final right‑boundary check. */
    seq_t ans = 0;
    for (uint32_t prev = 0; prev < 32; ++prev) {
        for (uint32_t curr = 0; curr < 32; ++curr) {
            if (_allowed_right[prev][curr])
                ans += dp[prev][curr];
        }
    }
    return ans;
}
