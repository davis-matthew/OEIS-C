/*
 * A297745  Number of nX4 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 1, 3 or 4 neighboring 1s.
 * https://oeis.org/A297745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by dynamic programming over rows: for each new row
 * check that the previous row satisfies the neighbour‑count rule
 * (1,3 or 4 neighbours counted over horizontal and diagonal
 * directions, vertical neighbours are not considered).  All
 * 4×n binary arrays are counted. */

#include <stdbool.h>
#include <stdint.h>

/* Helper: verify that every 1 in 'cur' has 1, 3 or 4 neighbours among
 * horizontal (left/right) and diagonal (top‑left, top‑right,
 * bottom‑left, bottom‑right) cells.  The rows 'above' and 'below'
 * are given as 4‑bit bit patterns (bit 0 = column 0). */
static inline bool check_row(uint8_t above, uint8_t cur, uint8_t below) {
    for (int col = 0; col < 4; ++col) {
        if ((cur >> col) & 1U) {
            int cnt = 0;
            /* horizontal neighbours */
            if (col > 0   && (cur >> (col - 1)) & 1U) ++cnt;
            if (col < 3   && (cur >> (col + 1)) & 1U) ++cnt;
            /* diagonal neighbours */
            if (col > 0   && (above >> (col - 1)) & 1U) ++cnt;
            if (col < 3   && (above >> (col + 1)) & 1U) ++cnt;
            if (col > 0   && (below >> (col - 1)) & 1U) ++cnt;
            if (col < 3   && (below >> (col + 1)) & 1U) ++cnt;
            if (cnt != 1 && cnt != 3 && cnt != 4) return false;
        }
    }
    return true;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;

    /* dp[prev2][prev1] = number of ways to build rows up to prev1
     * such that prev1 has not yet been validated. */
    static seq_t dp[16][16], nextdp[16][16];
    for (unsigned i = 0; i < 16; ++i)
        for (unsigned j = 0; j < 16; ++j)
            dp[i][j] = 0;
    dp[0][0] = 1;          /* two dummy zero rows above the first real row */

    for (idx_t r = 0; r < n; ++r) {
        for (unsigned i = 0; i < 16; ++i)
            for (unsigned j = 0; j < 16; ++j)
                nextdp[i][j] = 0;

        for (unsigned prev2 = 0; prev2 < 16; ++prev2)
            for (unsigned prev1 = 0; prev1 < 16; ++prev1) {
                seq_t cur = dp[prev2][prev1];
                if (cur == 0) continue;
                for (unsigned currow = 0; currow < 16; ++currow) {
                    /* prev1 (row r‑1) must be valid given prev2 and currow */
                    if (!check_row(prev2, prev1, currow)) continue;
                    nextdp[prev1][currow] += cur;
                }
            }

        for (unsigned i = 0; i < 16; ++i)
            for (unsigned j = 0; j < 16; ++j)
                dp[i][j] = nextdp[i][j];
    }

    /* Final dummy bottom row: validate the last real row */
    for (unsigned i = 0; i < 16; ++i)
        for (unsigned j = 0; j < 16; ++j)
            nextdp[i][j] = 0;

    for (unsigned prev2 = 0; prev2 < 16; ++prev2) {
        for (unsigned prev1 = 0; prev1 < 16; ++prev1) {
            seq_t cur = dp[prev2][prev1];
            if (cur == 0) continue;
            if (!check_row(prev2, prev1, 0)) continue; /* bottom dummy row = 0 */
            nextdp[prev1][0] += cur;
        }
    }

    seq_t total = 0;
    for (unsigned prev2 = 0; prev2 < 16; ++prev2)
        total += nextdp[prev2][0];
    return total;
}
