/*
 * A298085  Number of n X 6 0..1 arrays with every element equal to 2, 3, 5 or
 *          6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   row_satisfied
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: This function counts the number of n × 6 binary arrays
 * (0/1) where every cell has a number of equal‑valued king‑move
 * (Moore) neighbouring cells in the set {2,3,5,6}.
 * The upper‑left cell (row 0, column 0) is fixed to 0.
 *
 * The algorithm uses a transfer‑matrix dynamic programming over
 * row patterns. For each pair of consecutive rows (a,b) we keep all
 * ways to reach that state such that row a has already satisfied
 * its adjacency constraints. A transition to a new row c
 * is possible iff row b satisfies its constraints when its upper
 * neighbours are given by a and lower neighbours by c.
 *
 * Complexity:  O(n · 64³) precomputation + O(n · 64²) DP,
 * which is well below the 2‑second limit for n up to 10 000.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/row_satisfied.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* not used, but safe */

    /* Transition mask: adj[a][b] has bits for all c such that
     * row b satisfies constraints with above a and below c. */
    static uint64_t adj[64][64];
    static bool precomp = false;

    if (!precomp) {
        for (int a = 0; a < 64; ++a) {
            for (int b = 0; b < 64; ++b) {
                uint64_t mask = 0;
                for (int c = 0; c < 64; ++c) {
                    if (row_satisfied(a, b, c))
                        mask |= ((uint64_t)1 << c);
                }
                adj[a][b] = mask;
            }
        }
        precomp = true;
    }

    /* DP arrays: dp[prev][cur] counts ways up to current row cur
     * where prev is the row before cur, and cur's constraints already
     * satisfied.  We need only two layers. */
    static seq_t dp[64][64];
    static seq_t dp_next[64][64];

    /* Handle n = 1 separately (no previous/next rows). */
    if (n == 1) {
        seq_t res = 0;
        for (int r = 0; r < 64; ++r) {
            if ((r & 1) == 0 && row_satisfied(-1, r, -1))
                res += 1;
        }
        return res;
    }

    /* Initialize dp for row 2 (after adding rows 1 and 2). */
    for (int a = 0; a < 64; ++a)
        for (int b = 0; b < 64; ++b)
            dp[a][b] = 0;

    for (int r1 = 0; r1 < 64; ++r1) {
        if ((r1 & 1) != 0) continue;          /* upper‑left must be 0 */
        for (int r2 = 0; r2 < 64; ++r2) {
            if (row_satisfied(-1, r1, r2))
                dp[r1][r2] += 1;
        }
    }

    /* DP over rows 3..n */
    for (idx_t row = 3; row <= n; ++row) {
        /* Zero dp_next */
        for (int a = 0; a < 64; ++a)
            for (int b = 0; b < 64; ++b)
                dp_next[a][b] = 0;

        for (int a = 0; a < 64; ++a) {
            for (int b = 0; b < 64; ++b) {
                seq_t cur_val = dp[a][b];
                if (cur_val == 0) continue;
                uint64_t mask = adj[a][b];
                while (mask) {
                    uint64_t t = mask & -mask;
                    int c = (int)__builtin_ctzll(mask);
                    dp_next[b][c] += cur_val;
                    mask ^= t;
                }
            }
        }
        /* swap dp and dp_next */
        for (int a = 0; a < 64; ++a)
            for (int b = 0; b < 64; ++b)
                dp[a][b] = dp_next[a][b];
    }

    /* Final row (row n) must satisfy its constraints with above
     * row (n-1) and below absent. */
    seq_t result = 0;
    for (int a = 0; a < 64; ++a) {
        for (int b = 0; b < 64; ++b) {
            seq_t val = dp[a][b];
            if (val == 0) continue;
            if (row_satisfied(a, b, -1))
                result += val;
        }
    }
    return result;
}
