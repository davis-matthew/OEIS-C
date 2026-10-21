/*
 * A295202  Number of n X 5 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 2 or 4 1's.
 * https://oeis.org/A295202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays where every 1 has 2 or 4 *
 *            horizontal/vertical neighbours that are 1. See OEIS A295202
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* DP over rows; 5 columns → 32 possible patterns per row. */

#include <stdint.h>

/* Check whether a middle row 'mid' is valid when its neighbours
 * 'up' (above) and 'down' (below) are known.  All patterns are
 * 5‑bit integers.  A 1 must have 2 or 4 adjacent 1s. */
static inline int
is_valid(uint8_t mid, uint8_t up, uint8_t down)
{
    for (int j = 0; j < 5; ++j) {
        if (!(mid & (1u << j)))
            continue;                     /* no 1 at this position – no restriction */

        int cnt = 0;
        /* left neighbour */
        if (j > 0 && (mid & (1u << (j - 1))))
            ++cnt;
        /* right neighbour */
        if (j < 4 && (mid & (1u << (j + 1))))
            ++cnt;
        /* up neighbour */
        if (up & (1u << j))
            ++cnt;
        /* down neighbour */
        if (down & (1u << j))
            ++cnt;

        if (cnt != 2 && cnt != 4)
            return 0;                /* fails the requirement */
    }
    return 1;                        /* all cells satisfy the rule */
}

/* Transition masks:  next_mask[up][mid]  has bit k set iff
 * a row with pattern k is valid when the previous two rows are
 * 'up' and 'mid'. */
static uint32_t next_mask[32][32];
static int ready = 0;

/* The main sequence function.  It computes a(n) for any n≥1
 * in O(n) time and constant additional memory. */
seq_t
a(idx_t n)
{
    if (!ready) {            /* fill transition table once */
        for (int up = 0; up < 32; ++up) {
            for (int mid = 0; mid < 32; ++mid) {
                uint32_t mask = 0;
                for (int newRow = 0; newRow < 32; ++newRow) {
                    if (is_valid((uint8_t)mid, (uint8_t)up, (uint8_t)newRow))
                        mask |= 1u << newRow;
                }
                next_mask[up][mid] = mask;
            }
        }
        ready = 1;
    }

    /* DP tables: dp[last][prev] = number of ways up to current row
     * with most recent rows 'last' (current) and 'prev' (next‑up). */
    static seq_t dp[32][32];
    static seq_t dp_next[32][32];

    /* initialise dp for the first row (row 1).  There is no row above,
     * so its pattern acts as 'prev' = 0. */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = 0;
    for (int pat = 0; pat < 32; ++pat)
        dp[pat][0] = 1;

    /* Special handling if only one row exists. */
    if (n == 1) {
        seq_t ans = 0;
        for (int last = 0; last < 32; ++last)
            if (is_valid((uint8_t)last, 0, 0))
                ans += dp[last][0];
        return ans;
    }

    /* Process rows 2 … n.  For each new row, build dp_next from dp. */
    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp_next[i][j] = 0;

        for (int prev = 0; prev < 32; ++prev) {
            for (int last = 0; last < 32; ++last) {
                seq_t ways = dp[last][prev];
                if (!ways)
                    continue;
                uint32_t mask = next_mask[prev][last];
                while (mask) {
                    int newRow = __builtin_ctz(mask);
                    dp_next[newRow][last] += ways;
                    mask &= mask - 1;    /* clear lowest set bit */
                }
            }
        }

        /* swap dp and dp_next */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = dp_next[i][j];
    }

    /* Final validation of the last row (down neighbour = 0). */
    seq_t ans = 0;
    for (int last = 0; last < 32; ++last) {
        for (int prev = 0; prev < 32; ++prev) {
            if (is_valid((uint8_t)last, (uint8_t)prev, 0))
                ans += dp[last][prev];
        }
    }
    return ans;
}
