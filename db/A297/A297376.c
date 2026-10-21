/*
 * A297376  Number of 3Xn 0..1 arrays with every 1 horizontally or
 *          antidiagonally adjacent to 0, 1 or 2 neighboring 1s.
 * https://oeis.org/A297376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Computation of OEIS sequence A297376
   Number of 3×n 0..1 arrays where every 1 has at most two
   horizontally or antidiagonally adjacent 1s.
   The values are obtained by dynamic programming over column
   states: each state is a pair of 3‑bit columns (0..7).
   Transition to a new column checks that the middle column
   satisfies the adjacency rule with its left and right neighbours.
   Complexity: O(n·512), memory: 64 seq_t cells. */
seq_t a(idx_t n) {
    /* Pre‑compute the validity table for triples (left,mid,right) */
    static unsigned char valid[8][8][8];
    static bool computed = false;
    if (!computed) {
        for (int left = 0; left < 8; left++) {
            for (int mid = 0; mid < 8; mid++) {
                for (int right = 0; right < 8; right++) {
                    bool ok = true;
                    for (int r = 0; r < 3 && ok; r++) {
                        if (!(mid & (1 << r))) continue;   /* only 1 cells */
                        int neigh = 0;
                        /* left horizontal */
                        if (left & (1 << r)) neigh++;
                        /* right horizontal */
                        if (right & (1 << r)) neigh++;
                        /* up-right anti‑diagonal: row r-1 in right column */
                        if (r > 0 && (right & (1 << (r - 1)))) neigh++;
                        /* down-left anti‑diagonal: row r+1 in left column */
                        if (r < 2 && (left & (1 << (r + 1)))) neigh++;
                        if (neigh > 2) ok = false;
                    }
                    valid[left][mid][right] = ok;
                }
            }
        }
        computed = true;
    }

    /* DP over pairs of columns (prev, curr) */
    static seq_t dp[8][8];
    static seq_t newdp[8][8];
    /* Initialise with the first column (prev = 0 imaginary) */
    for (int p = 0; p < 8; ++p)
        for (int c = 0; c < 8; ++c)
            dp[p][c] = 0;
    for (int c = 0; c < 8; ++c)
        dp[0][c] = 1;    /* one way to have first real column = c */

    /* Add columns 2..n */
    for (idx_t s = 1; s < n; ++s) {          /* s = current length after adding column s+1 */
        for (int p = 0; p < 8; ++p)
            for (int c = 0; c < 8; ++c)
                newdp[p][c] = 0;

        for (int prev = 0; prev < 8; ++prev) {
            for (int curr = 0; curr < 8; ++curr) {
                seq_t ways = dp[prev][curr];
                if (!ways) continue;
                for (int nxt = 0; nxt < 8; ++nxt) {
                    if (valid[prev][curr][nxt])
                        newdp[curr][nxt] += ways;
                }
            }
        }
        /* copy back to dp */
        for (int p = 0; p < 8; ++p)
            for (int c = 0; c < 8; ++c)
                dp[p][c] = newdp[p][c];
    }

    /* Verify the last column (right neighbor is 0) */
    seq_t result = 0;
    for (int prev = 0; prev < 8; ++prev) {
        for (int curr = 0; curr < 8; ++curr) {
            if (valid[prev][curr][0])
                result += dp[prev][curr];
        }
    }
    return result;
}
