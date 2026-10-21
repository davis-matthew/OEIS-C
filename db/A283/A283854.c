/*
 * A283854  Number of nX5 0..1 arrays with no 1 equal to more than three of
 *          its horizontal, diagonal and antidiagonal neighbors.
 * https://oeis.org/A283854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A283854: Number of n×5 binary arrays with no 1 having more than
 * three of its horizontal, diagonal and antidiagonal neighbors.
 * Implementation uses a transfer‑matrix DP over rows.  The state is a
 * pair of consecutive rows, and after adding a new row we verify the
 * middle row's constraint using the two rows on either side.
 * Complexity: O( n · 32³ ) ≈ O( n ) with a small constant.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handles n ≥ 1. */
    if (n < 1) return 0;

    /* Transition table: for each (prev, curr) the bitmask of next
       rows that satisfy the constraint for 'curr'.                 */
    static uint32_t allowedNext[32][32];
    static int initialized = 0;

    if (!initialized) {
        /* Precompute allowedNext. */
        for (int prev = 0; prev < 32; ++prev) {
            for (int curr = 0; curr < 32; ++curr) {
                uint32_t mask = 0;
                for (int next = 0; next < 32; ++next) {
                    /* Count neighbors of every 1 in 'curr' using
                       rows prev, curr, next. */
                    int ok = 1;
                    for (int j = 0; j < 5 && ok; ++j) {
                        if (!(curr & (1 << j))) continue; /* 0 cell: ignore */
                        int cnt = 0;
                        /* Left and right in the same row */
                        if (j > 0)   cnt += (curr >> (j - 1)) & 1;
                        if (j < 4)   cnt += (curr >> (j + 1)) & 1;
                        /* Diagonals from the upper row */
                        if (j > 0)   cnt += (prev >> (j - 1)) & 1;
                        if (j < 4)   cnt += (prev >> (j + 1)) & 1;
                        /* Diagonals from the lower row */
                        if (j > 0)   cnt += (next >> (j - 1)) & 1;
                        if (j < 4)   cnt += (next >> (j + 1)) & 1;
                        if (cnt > 3) ok = 0; /* violated */
                    }
                    if (ok) mask |= 1u << next;
                }
                allowedNext[prev][curr] = mask;
            }
        }
        initialized = 1;
    }

    /* DP arrays: dp[prev][curr] holds the number of ways for the
       already processed rows ending with pair (prev, curr). */
    static seq_t dp[32][32];
    static seq_t newdp[32][32];

    /* Zero initialization of dp. */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = 0;

    /* Initial state: phantom top row of zeros, followed by any first row. */
    for (int first = 0; first < 32; ++first)
        dp[0][first] = 1;

    /* Process rows 2 through n+1 (n+1 is the phantom bottom row). */
    for (idx_t step = 2; step <= n + 1; ++step) {
        /* Zero newdp. */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                newdp[i][j] = 0;

        for (int prev = 0; prev < 32; ++prev) {
            for (int curr = 0; curr < 32; ++curr) {
                seq_t ways = dp[prev][curr];
                if (ways == 0) continue;
                uint32_t mask = allowedNext[prev][curr];
                while (mask) {
                    int next = __builtin_ctz(mask);
                    mask &= mask - 1;          /* clear lowest set bit */
                    newdp[curr][next] += ways;
                }
            }
        }
        /* Copy newdp back to dp for the next iteration. */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* Sum over all states where the last row is the phantom bottom row (0). */
    seq_t result = 0;
    for (int prev = 0; prev < 32; ++prev)
        result += dp[prev][0];

    return result;
}
