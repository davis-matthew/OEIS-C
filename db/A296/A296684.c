/*
 * A296684  Number of nX4 0..1 arrays with each 1 adjacent to 1, 2 or 3
 *          king-move neighboring 1s.
 * https://oeis.org/A296684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of n×4 0..1 arrays with each 1 adjacent to 1, 2 or
 *            3 king‑move neighboring 1s
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   neigh_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a transfer‑matrix DP over pairs of rows (prevprev, prev). */
/* The state space has 16×16=256 states, and each step processes 16 possible new rows. */
/* Complexity: O(n) with a small constant (~4 k transitions per row), well under the limits for n≤10000. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/neigh_row.h"

/* Precomputed horizontal neighbor counts for each 4‑bit row and column. */
static int horiz[16][4];

/* Validate that all 1‑cells in 'prev' have 1–3 neighbors using 'prevprev' (above) and 'curr' (below). */
static inline bool validate_row(int prevprev, int prev, int curr)
{
    if (prev == 0) return true;   /* no 1‑cells to check */
    for (int j = 0; j < 4; ++j) {
        if (prev & (1 << j)) {
            int total = horiz[prev][j];
            total += neigh_row(prevprev, j);
            total += neigh_row(curr,     j);
            if (total < 1 || total > 3) return false;
        }
    }
    return true;
}

/* Validate that all 1‑cells in 'curr' (the last row) have 1–3 neighbors using 'prev' (above). */
static inline bool final_row_valid(int prev, int curr)
{
    if (curr == 0) return true;   /* no 1‑cells to check */
    for (int j = 0; j < 4; ++j) {
        if (curr & (1 << j)) {
            int total = horiz[curr][j];
            total += neigh_row(prev, j);
            if (total < 1 || total > 3) return false;
        }
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Convert idx_t to size_t for loop limits if needed */
    if (n < 1) return (seq_t)0;   /* undefined for n==0 */

    /* Precompute horizontal neighbors once (static initialization) */
    static bool horiz_done = false;
    if (!horiz_done) {
        for (int mask = 0; mask < 16; ++mask) {
            for (int j = 0; j < 4; ++j) {
                horiz[mask][j] = 0;
                if (j > 0 && (mask & (1 << (j - 1)))) horiz[mask][j]++;
                if (j < 3 && (mask & (1 << (j + 1)))) horiz[mask][j]++;
            }
        }
        horiz_done = true;
    }

    /* DP arrays: dp[prevprev][prev] holds counts after current number of rows processed. */
    static seq_t dp[16][16];
    static seq_t next[16][16];

    /* Initialize DP for zero rows: only state (0,0) is 1. */
    memset(dp, 0, sizeof(dp));
    dp[0][0] = (seq_t)1;

    /* Iterate over rows 1..n */
    for (idx_t row = 1; row <= n; ++row) {
        /* Zero next DP */
        memset(next, 0, sizeof(next));

        /* Transition from each existing state to all possible new rows */
        for (int prevprev = 0; prevprev < 16; ++prevprev) {
            for (int prev = 0; prev < 16; ++prev) {
                seq_t ways = dp[prevprev][prev];
                if (ways == (seq_t)0) continue;
                for (int curr = 0; curr < 16; ++curr) {
                    if (validate_row(prevprev, prev, curr)) {
                        next[prev][curr] += ways;
                    }
                }
            }
        }

        /* Swap dp and next for next iteration */
        memcpy(dp, next, sizeof(dp));
    }

    /* After processing n rows, sum over states where the last row is valid */
    seq_t result = (seq_t)0;
    for (int prev = 0; prev < 16; ++prev) {
        for (int curr = 0; curr < 16; ++curr) {
            if (dp[prev][curr] == (seq_t)0) continue;
            if (final_row_valid(prev, curr))
                result += dp[prev][curr];
        }
    }
    return result;
}
