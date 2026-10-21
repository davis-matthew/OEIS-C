/*
 * A299356  Number of nX5 0..1 arrays with every element equal to 0, 1, 2, 3,
 *          6 or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 15, u64 n <= 32, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of n×5 binary arrays with king‑move adjacency constraints,
   upper‑left element zero.  The value a(n) is obtained by a dynamic
   programming over rows.  For each row we keep track of the previous
   row so that validity of a cell (with at most two neighbouring rows)
   can be checked.  A special mask value 32 represents a non‑existent
   row (used for the top and bottom boundaries). */
#include <stdint.h>
#include <stdbool.h>

/* Allowed number of equal king‑move neighbours for a cell */
static const bool allowed[9] = { true, true, true, true, false,
                                 false, true, false, true };

/* Returns true iff the configuration (prev,curr,next) is valid.
   The mask value 32 represents a missing row (outside the array). */
static bool is_valid_triplet(uint8_t prev, uint8_t curr, uint8_t next)
{
    for (int col = 0; col < 5; ++col) {
        int cur = (curr >> col) & 1;
        int cnt = 0;

        /* neighbours in the previous row */
        if (prev != 32) {
            for (int dc = -1; dc <= 1; ++dc) {
                int nc = col + dc;
                if (0 <= nc && nc < 5) {
                    int bit = (prev >> nc) & 1;
                    if (bit == cur) ++cnt;
                }
            }
        }

        /* neighbours in the next row */
        if (next != 32) {
            for (int dc = -1; dc <= 1; ++dc) {
                int nc = col + dc;
                if (0 <= nc && nc < 5) {
                    int bit = (next >> nc) & 1;
                    if (bit == cur) ++cnt;
                }
            }
        }

        /* same‑row neighbours */
        if (col > 0)   { int b = (curr >> (col-1)) & 1; if (b == cur) ++cnt; }
        if (col < 4)   { int b = (curr >> (col+1)) & 1; if (b == cur) ++cnt; }

        if (!allowed[cnt])
            return false;
    }
    return true;
}

/* DP over rows.  The state is (prev_row, curr_row).  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* not used, offset is 1 */

    /* DP arrays: prev index 0..32 (32 means no row), curr 0..31 */
    static seq_t dp[33][32], ndp[33][32];

    /* initialise to zero */
    for (int i = 0; i < 33; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = 0;

    /* first row: previous row is non‑existent (32) */
    for (uint8_t mask = 0; mask < 32; ++mask) {
        if ((mask & 1) != 0) continue;         /* upper‑left must be 0 */
        dp[32][mask] = 1;
    }

    /* iterate over remaining rows */
    for (idx_t row = 1; row < n; ++row) {
        /* clear ndp */
        for (int i = 0; i < 33; ++i)
            for (int j = 0; j < 32; ++j)
                ndp[i][j] = 0;

        for (int prev = 0; prev <= 32; ++prev) {
            for (int curr = 0; curr < 32; ++curr) {
                seq_t val = dp[prev][curr];
                if (val == 0) continue;
                for (int next = 0; next < 32; ++next) {
                    if (is_valid_triplet(prev, curr, next))
                        ndp[curr][next] += val;
                }
            }
        }

        /* swap dp and ndp by copying */
        for (int i = 0; i < 33; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* final row: next row is non‑existent (32) */
    seq_t result = 0;
    for (int prev = 0; prev <= 32; ++prev) {
        for (int curr = 0; curr < 32; ++curr) {
            seq_t val = dp[prev][curr];
            if (val == 0) continue;
            if (is_valid_triplet(prev, curr, 32))
                result += val;
        }
    }
    return result;
}
