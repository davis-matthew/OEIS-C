/*
 * A279462  Number of n X 4 0..1 arrays with no element equal to a strict
 *          majority of its horizontal, vertical and antidiagonal neighbors,
 *          with the exception of exactly one element, and with new values
 *          introduced in order 0 sequentially upwards.
 * https://oeis.org/A279462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary arrays that contain exactly one cell
 *            whose * value is a strict majority of its horizontal, vertical
 *            and antidiagonal * neighbors, divided by two to account for the
 *            complementary symmetry * (bitwise 0↔1). The DP described below
 *            counts all arrangements; the final * divis...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define COLS 4
#define NONE_MASK 16                /* index for a nonexistent row */

static bool table_ready = false;
static unsigned viol_table[17][16][17];  /* [prev][curr][next] */

/* Pre‑compute the number of violations in a row, given the previous,
 * current, and next row masks.  The table is indexed as viol_table[prev][curr][next].
 * Use 16 to denote a “non‑existent” row. */
static void precompute_viol_table(void)
{
    if (table_ready) return;

    for (int p = 0; p <= NONE_MASK; ++p) {
        bool hasPrev = (p != NONE_MASK);
        for (int c = 0; c < 16; ++c) {
            for (int k = 0; k <= NONE_MASK; ++k) {
                bool hasNext = (k != NONE_MASK);
                unsigned viol = 0;

                for (int j = 0; j < COLS; ++j) {
                    int ones = 0;
                    int S = 0;               /* number of neighbors */
                    int currBit = (c >> j) & 1;

                    /* Left */
                    if (j > 0) {
                        ++S;
                        if ((c >> (j - 1)) & 1) ++ones;
                    }
                    /* Right */
                    if (j < COLS - 1) {
                        ++S;
                        if ((c >> (j + 1)) & 1) ++ones;
                    }
                    /* Up */
                    if (hasPrev) {
                        ++S;
                        if ((p >> j) & 1) ++ones;
                    }
                    /* Down */
                    if (hasNext) {
                        ++S;
                        if ((k >> j) & 1) ++ones;
                    }
                    /* Up‑left antidiagonal */
                    if (hasPrev && j > 0) {
                        ++S;
                        if ((p >> (j - 1)) & 1) ++ones;
                    }
                    /* Down‑right antidiagonal */
                    if (hasNext && j < COLS - 1) {
                        ++S;
                        if ((k >> (j + 1)) & 1) ++ones;
                    }

                    int zeros = S - ones;
                    bool strictMajority = (ones * 2 > S) || (zeros * 2 > S);

                    if (!strictMajority) continue;          /* no majority, cannot be violated */

                    int majorityBit = (ones > zeros) ? 1 : 0;
                    if (currBit == majorityBit) ++viol;
                }
                viol_table[p][c][k] = viol;
            }
        }
    }
    table_ready = true;
}

seq_t a(idx_t n)
{
    precompute_viol_table();

    /* DP tables: dp[prev_mask][curr_mask][violations] where violations∈{0,1} */
    static seq_t dp[17][16][2];
    static seq_t newdp[17][16][2];

    /* Zero dp */
    for (int p = 0; p <= NONE_MASK; ++p)
        for (int c = 0; c < 16; ++c)
            for (int v = 0; v < 2; ++v)
                dp[p][c][v] = 0;

    /* First row, no previous row */
    for (int r = 0; r < 16; ++r)
        dp[NONE_MASK][r][0] = 1;

    /* Process subsequent rows */
    for (idx_t row = 1; row < n; ++row) {
        /* Zero newdp */
        for (int p = 0; p <= NONE_MASK; ++p)
            for (int c = 0; c < 16; ++c)
                for (int v = 0; v < 2; ++v)
                    newdp[p][c][v] = 0;

        for (int p = 0; p <= NONE_MASK; ++p) {
            for (int c = 0; c < 16; ++c) {
                for (int v = 0; v < 2; ++v) {
                    seq_t cur = dp[p][c][v];
                    if (!cur) continue;
                    for (int next = 0; next < 16; ++next) {
                        unsigned add = viol_table[p][c][next];
                        int total = v + add;
                        if (total > 1) continue;
                        newdp[c][next][total] += cur;
                    }
                }
            }
        }

        /* Copy newdp back into dp */
        for (int p = 0; p <= NONE_MASK; ++p)
            for (int c = 0; c < 16; ++c)
                for (int v = 0; v < 2; ++v)
                    dp[p][c][v] = newdp[p][c][v];
    }

    /* Final row: no next row */
    seq_t ans = 0;
    for (int p = 0; p <= NONE_MASK; ++p) {
        for (int c = 0; c < 16; ++c) {
            for (int v = 0; v < 2; ++v) {
                seq_t cur = dp[p][c][v];
                if (!cur) continue;
                unsigned last = viol_table[p][c][NONE_MASK];
                int total = v + last;
                if (total == 1) {
                    ans += cur;
                }
            }
        }
    }

    /* Adjust for bitwise complement symmetry */
    ans >>= 1;          /* divide by 2 */
    return ans;
}
