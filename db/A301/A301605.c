/*
 * A301605  Number of nX5 0..1 arrays with every element equal to 0, 2 or 3
 *          horizontally or vertically adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sequence computed by dynamic programming over rows. For each
 *            cell, the number of horizontally or vertically adjacent elements
 *            equal to it must be 0, 2 or 3. The top‑left element is 0
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* get bit at column col (0 = leftmost, 4 = rightmost) */
static inline unsigned bit_at(uint8_t row, unsigned col) {
    return (row >> (4 - col)) & 1u;
}

/* Check that for every element in 'row', the count of adjacent elements
   that are equal to it is in {0,2,3}. 'above' and 'below' hold the rows
   above and below if 'have_above'/'have_below' are true. */
static bool is_valid_eq(uint8_t row, uint8_t above, bool have_above,
                        uint8_t below, bool have_below)
{
    for (unsigned col = 0; col < 5; ++col) {
        unsigned cnt = 0;
        unsigned cur = bit_at(row, col);
        /* left neighbour */
        if (col > 0 && bit_at(row, col - 1) == cur) ++cnt;
        /* right neighbour */
        if (col < 4 && bit_at(row, col + 1) == cur) ++cnt;
        /* up neighbour */
        if (have_above && bit_at(above, col) == cur) ++cnt;
        /* down neighbour */
        if (have_below && bit_at(below, col) == cur) ++cnt;
        /* allowed counts are 0, 2, or 3 */
        if (cnt == 1 || cnt == 4) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Offset is 1: a(1) is the first term. */
    if (n == 0) return 0;          /* not used, harmless */

    if (n == 1) {
        seq_t ans = 0;
        for (uint8_t mask = 0; mask < 32; ++mask) {
            if (bit_at(mask, 0) != 0) continue;       /* upper‑left zero */
            if (is_valid_eq(mask, 0, false, 0, false)) {
                ans += (seq_t)1;
            }
        }
        return ans;
    }

    /* pre‑compute whether a triple (prev, curr, next) is compatible */
    static bool next_ok[32][32][32];
    static int init = 0;
    if (!init) {
        for (int prev = 0; prev < 32; ++prev) {
            for (int curr = 0; curr < 32; ++curr) {
                for (int next = 0; next < 32; ++next) {
                    next_ok[prev][curr][next] =
                        is_valid_eq((uint8_t)curr,
                                    (uint8_t)prev, true,
                                    (uint8_t)next, true);
                }
            }
        }
        init = 1;
    }

    /* DP state: dp[prev][curr] = number of ways up to the current row
       where the last two rows are 'prev' (row-1) and 'curr' (row). */
    seq_t dp[32][32] = {0}, ndp[32][32];

    /* Initialise for rows 1 and 2 */
    for (int r1 = 0; r1 < 32; ++r1) {
        if (bit_at(r1, 0) != 0) continue;          /* upper‑left must be 0 */
        for (int r2 = 0; r2 < 32; ++r2) {
            if (is_valid_eq((uint8_t)r1, 0, false, (uint8_t)r2, true))
                dp[r1][r2] += (seq_t)1;
        }
    }

    /* Process rows 3 … n */
    for (idx_t row = 3; row <= n; ++row) {
        /* zero ndp */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                ndp[i][j] = 0;

        for (int prev = 0; prev < 32; ++prev) {
            for (int curr = 0; curr < 32; ++curr) {
                seq_t cur_val = dp[prev][curr];
                if (cur_val == 0) continue;
                for (int next = 0; next < 32; ++next) {
                    if (next_ok[prev][curr][next]) {
                        ndp[curr][next] += cur_val;
                    }
                }
            }
        }
        /* move to next step */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Sum over the final row configurations. */
    seq_t ans = 0;
    for (int prev = 0; prev < 32; ++prev) {
        for (int curr = 0; curr < 32; ++curr) {
            seq_t val = dp[prev][curr];
            if (val == 0) continue;
            if (is_valid_eq((uint8_t)curr,
                            (uint8_t)prev, true, 0, false))
                ans += val;
        }
    }
    return ans;
}
