/*
 * A299810  Number of n X 4 0..1 arrays with every element equal to 0, 1, 3,
 *          4, 5, 6 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A299810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by a transfer‑matrix DP over 4‑wide binary
 *            rows, counting arrays that satisfy the king‑move adjacency
 *            restriction
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   _valid_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/_valid_row.h"

/* The main function required by the build. */
seq_t a(idx_t n)
{
    /* Precomputed tables used in the DP. */
    static bool tables_done = false;
    static bool valid_both[16][16][16];
    static bool valid_no_prev[16][16];
    static bool valid_no_next[16][16];
    static bool valid_zero[16];

    if (!tables_done) {
        /* valid_both: both neighbours exist. */
        for (int p = 0; p < 16; ++p)
            for (int c = 0; c < 16; ++c) {
                for (int q = 0; q < 16; ++q)
                    valid_both[p][c][q] = _valid_row(p, c, q, true, true);
                valid_no_next[p][c] = _valid_row(p, c, 0, true, false);
            }
        for (int c = 0; c < 16; ++c)
            for (int q = 0; q < 16; ++q)
                valid_no_prev[c][q] = _valid_row(0, c, q, false, true);
        for (int c = 0; c < 16; ++c)
            valid_zero[c] = _valid_row(0, c, 0, false, false);
        tables_done = true;
    }

    if (n == 1) {                     /* special case: only one row */
        seq_t ans = 0;
        for (int c = 0; c < 16; ++c)
            if ((c & 1) == 0 && valid_zero[c])
                ans += 1;
        return ans;
    }

    /* dp[prev][curr] holds the number of ways for the already processed
       part of the array, where 'prev' is row (r‑1) and 'curr' is row r.   */
    static seq_t dp[16][16], newdp[16][16];
    for (int p = 0; p < 16; ++p)
        for (int c = 0; c < 16; ++c)
            dp[p][c] = 0;

    /* First row – no previous neighbour, but must have upper‑left zero. */
    for (int c = 0; c < 16; ++c)
        if ((c & 1) == 0)
            dp[0][c] = 1;

    /* Process rows 2 … n. */
    for (idx_t r = 2; r <= n; ++r) {
        for (int p = 0; p < 16; ++p)
            for (int c = 0; c < 16; ++c)
                newdp[p][c] = 0;

        if (r == 2) {   /* transition from the imaginary previous row */
            for (int c = 0; c < 16; ++c) {
                seq_t ways = dp[0][c];
                if (!ways) continue;
                for (int q = 0; q < 16; ++q)
                    if (valid_no_prev[c][q])
                        newdp[c][q] += ways;
            }
        } else {        /* normal transition */
            for (int p = 0; p < 16; ++p)
                for (int c = 0; c < 16; ++c) {
                    seq_t ways = dp[p][c];
                    if (!ways) continue;
                    for (int q = 0; q < 16; ++q)
                        if (valid_both[p][c][q])
                            newdp[c][q] += ways;
                }
        }

        /* swap dp and newdp */
        for (int p = 0; p < 16; ++p)
            for (int c = 0; c < 16; ++c)
                dp[p][c] = newdp[p][c];
    }

    /* Final row – no succeeding neighbour. */
    seq_t ans = 0;
    for (int p = 0; p < 16; ++p)
        for (int c = 0; c < 16; ++c) {
            seq_t ways = dp[p][c];
            if (!ways) continue;
            if (valid_no_next[p][c])
                ans += ways;
        }

    return ans;
}
