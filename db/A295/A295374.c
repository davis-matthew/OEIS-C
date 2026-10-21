/*
 * A295374  Number of n X 3 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 0, 2 or 4 1s.
 * https://oeis.org/A295374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n × 3 0..1 arrays where every 1 has an even
 *            number (0, 2, or 4) of horizontally or vertically adjacent 1s.
 *            This implementation uses a transfer‑matrix DP over pairs of
 *            consecutive rows, processing all rows sequentially. For each n ≥
 *            1, a(idx_t n) returns the required count
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Test whether a single row satisfies the adjacency rule when surrounded
   by a row above and a row below.  The function works for all 3‑bit row
   states (0..7).  The ‘above’ and ‘below’ parameters are 3‑bit patterns
   themselves.  Each cell in ‘row’ is checked: the number of neighboring
   1s (left, right, above, below) must be even. */
static inline bool row_ok(uint8_t row, uint8_t above, uint8_t below)
{
    for (int j = 0; j < 3; ++j) {
        if (!(row & (1U << j))) continue;          /* skip 0 cells */
        int sum = 0;
        if (j > 0 && (row & (1U << (j - 1)))) ++sum;   /* left */
        if (j < 2 && (row & (1U << (j + 1)))) ++sum;   /* right */
        if (above   & (1U << j))                ++sum;   /* above */
        if (below   & (1U << j))                ++sum;   /* below */
        if (sum & 1) return false;               /* odd neighbour count */
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Handle the offset (n = 1 gives 5). */
    if (n == 1) {
        seq_t ans = 0;
        for (int r = 0; r < 8; ++r)
            if (row_ok((uint8_t)r, 0, 0))
                ++ans;
        return ans;
    }

    /* DP tables: dp[prev][curr] counts sequences whose last two rows are
       prev and curr.  The first row is represented by a virtual row 0. */
    seq_t dp[8][8]   = {{0}};
    seq_t next_dp[8][8] = {{0}};

    /* Initialise after the first row.  The "previous previous" row is the
       virtual row 0. */
    for (int r = 0; r < 8; ++r)
        dp[0][r] = 1;

    /* Process rows 2 … n.  After each iteration, dp will hold the
       distribution for the last two rows. */
    for (idx_t i = 2; i <= n; ++i) {
        /* zero next_dp */
        for (int a = 0; a < 8; ++a)
            for (int b = 0; b < 8; ++b)
                next_dp[a][b] = 0;

        for (int a = 0; a < 8; ++a)          /* a = prevPrev row */
            for (int b = 0; b < 8; ++b) {     /* b = prev row */
                seq_t cnt = dp[a][b];
                if (!cnt) continue;
                for (int c = 0; c < 8; ++c) { /* c = new row */
                    if (row_ok((uint8_t)b, (uint8_t)a, (uint8_t)c))
                        next_dp[b][c] += cnt;   /* new state: prev=b, curr=c */
                }
            }

        /* copy next_dp into dp for the next iteration */
        for (int a = 0; a < 8; ++a)
            for (int b = 0; b < 8; ++b)
                dp[a][b] = next_dp[a][b];
    }

    /* Final validation: the last row (prev) has no row below.
       For each stored state (prevPrev, prev) we check that
       row_ok(prev, prevPrev, 0) holds. */
    seq_t ans = 0;
    for (int prevPrev = 0; prevPrev < 8; ++prevPrev) {
        for (int prev = 0; prev < 8; ++prev) {
            seq_t cnt = dp[prevPrev][prev];
            if (!cnt) continue;
            if (row_ok((uint8_t)prev, (uint8_t)prevPrev, 0))
                ans += cnt;
        }
    }
    return ans;
}
