/*
 * A298626  Number of nX5 0..1 arrays with every element equal to 2, 3, 4, 5
 *          or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of n×5 binary arrays with upper-left zero
 * and each cell having 2, 3, 4, 5 or 7 equal king‑move neighbors.
 * The method counts all such arrays by dynamic programming over rows.
 * The DP state is a pair of consecutive rows; transitions are precomputed.
 * A shallow copy of DP tables via assignment avoids double‑free of
 * internally allocated big‑integer data.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Helper: test whether a row `curr` is valid given its neighbours.
 * hasPrev/hasNext indicate presence of a preceding/next row (used for border rows).
 */
static bool row_is_valid(uint32_t prev, uint32_t curr, uint32_t next,
                         bool hasPrev, bool hasNext)
{
    for (int col = 0; col < 5; ++col) {
        int currVal = (curr >> col) & 1;
        int cnt = 0;
        if (hasPrev) {
            if (col > 0 && ((prev >> (col - 1)) & 1) == currVal) ++cnt;   // up‑left
            if (((prev >> col) & 1) == currVal) ++cnt;                     // up
            if (col < 4 && ((prev >> (col + 1)) & 1) == currVal) ++cnt;   // up‑right
        }
        if (col > 0 && ((curr >> (col - 1)) & 1) == currVal) ++cnt;        // left
        if (col < 4 && ((curr >> (col + 1)) & 1) == currVal) ++cnt;        // right
        if (hasNext) {
            if (col > 0 && ((next >> (col - 1)) & 1) == currVal) ++cnt;   // down‑left
            if (((next >> col) & 1) == currVal) ++cnt;                     // down
            if (col < 4 && ((next >> (col + 1)) & 1) == currVal) ++cnt;   // down‑right
        }
        /* Allowed counts: 2,3,4,5,7. Reject all others. */
        if (cnt == 0 || cnt == 1 || cnt == 6 || cnt == 8) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Offset is 1 */
    if (n <= 1) return 0;

    /* Cached transition tables – built once. */
    static bool initialized = false;
    static bool top_valid[32][32];
    static bool bottom_valid[32][32];
    static uint32_t trans_next_mask[32][32];

    if (!initialized) {
        /* Top boundary: no above row, below present */
        for (int r1 = 0; r1 < 32; ++r1)
            for (int r2 = 0; r2 < 32; ++r2)
                top_valid[r1][r2] = row_is_valid(0, r1, r2, false, true);

        /* Bottom boundary: above present, no below row */
        for (int r1 = 0; r1 < 32; ++r1)
            for (int r2 = 0; r2 < 32; ++r2)
                bottom_valid[r1][r2] = row_is_valid(r1, r2, 0, true, false);

        /* Interior transitions */
        for (int prev = 0; prev < 32; ++prev)
            for (int curr = 0; curr < 32; ++curr) {
                uint32_t mask = 0;
                for (int next = 0; next < 32; ++next)
                    if (row_is_valid(prev, curr, next, true, true))
                        mask |= (uint32_t)1 << next;
                trans_next_mask[prev][curr] = mask;
            }
        initialized = true;
    }

    /* DP tables */
    static seq_t dp[32][32];
    static seq_t dp_next[32][32];

    /* Reset dp */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = 0;

    /* Base: first two rows */
    for (int r1 = 0; r1 < 32; ++r1) {
        if ((r1 & 1) != 0) continue;   /* top-left must be 0 */
        for (int r2 = 0; r2 < 32; ++r2)
            if (top_valid[r1][r2])
                dp[r1][r2] = 1;
    }

    /* Process rows 3 .. n */
    for (idx_t rowIndex = 3; rowIndex <= n; ++rowIndex) {
        /* Clear dp_next */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp_next[i][j] = 0;

        /* Transition */
        for (int prev = 0; prev < 32; ++prev)
            for (int curr = 0; curr < 32; ++curr) {
                seq_t val = dp[prev][curr];
                if (val == 0) continue;
                uint32_t mask = trans_next_mask[prev][curr];
                while (mask) {
                    int pos = __builtin_ctz(mask);
                    mask &= mask - 1;
                    dp_next[curr][pos] += val;
                }
            }

        /* Copy dp_next → dp via assignment (avoids memcpy of internal pointers) */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = dp_next[i][j];
    }

    /* Sum over valid bottom rows */
    seq_t result = 0;
    for (int prev = 0; prev < 32; ++prev)
        for (int curr = 0; curr < 32; ++curr)
            if (bottom_valid[prev][curr])
                result += dp[prev][curr];

    return result;
}
