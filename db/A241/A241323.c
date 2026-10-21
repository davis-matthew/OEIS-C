/*
 * A241323  Number of nX2 0..3 arrays with no element equal to fewer vertical
 *          neighbors than horizontal neighbors, with new values 0..3
 *          introduced in row major order.
 * https://oeis.org/A241323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   row_code, row_v0, row_v1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) counts n×2 arrays over {0,1,2,3} with the property that
 * each entry is not allowed to have fewer vertical neighbors equal to it
 * than horizontal neighbors.  The 2‑column arrays are read in row‑major
 * order and the first appearance of the values 0,1,2,3 must occur in
 * increasing order (new values are introduced in order).
 *
 * The implementation uses dynamic programming:
 *   - A state is (row i‑1, row i‑2, seen‑mask).
 *   - For each state we try all possible current rows that satisfy the
 *     neighbour condition with row i‑1 and row i‑2.
 *     The seen‑mask is updated according to the row‑major rule.
 *   - After processing all rows the final row must satisfy its own
 *     condition using only the row above (there is no below neighbour).
 *   - The Count is stored in `seq_t` for arbitrary precision or a fixed
 *     unsigned integer type.
 *
 * Complexity: O(n · #states · avg_successors),
 *   where #states = 16·17·16 = 4352 and avg_successors ≈ 4–5.
 * It easily meets the 2‑second limit for n up to 10 000.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/row_code.h"
#include "../utils/row_v0.h"
#include "../utils/row_v1.h"

#define NULL_ROW 16          /* sentinel for “no row above” */
#define INVALID_MASK 255U    /* sentinel for invalid mask transition */

/*
 * Precompute two lookup tables:
 *   1. next_mask[mask][row] -> new mask after inserting a row,
 *      or INVALID_MASK if the row violates the “new values in order”
 *      rule.
 *   2. allowed_cont[curr_row][prev_row][prevprev] -> list of rows
 *      that satisfy the neighbour condition.
 */
static bool tables_prepared = false;

/* Tables */
static unsigned char next_mask[16][16];
/* allowed codes: up to 16 rows per (prev, prevprev) pair */
static uint8_t allowed_cnt[16][17];
static uint8_t allowed_codes[16][17][16];

/* --------------------------------------------------------------- */
/* Construct the lookup tables on first call */
static void prepare_tables(void)
{
    if (tables_prepared) return;
    tables_prepared = true;

    /* ---- next_mask table ---- */
    for (int mask = 0; mask < 16; ++mask) {
        for (int rc = 0; rc < 16; ++rc) {
            int m = mask;
            int v0 = row_v0(rc);
            int v1 = row_v1(rc);
            int orig_m = m;
            /* process first cell (row‑major) */
            if ((m >> v0) & 1) {
                /* already seen */
            } else {
                /* new value: must be the minimal unseen one */
                int min_missing = 0;
                while ((m >> min_missing) & 1) ++min_missing;
                if (v0 != min_missing) { next_mask[mask][rc] = INVALID_MASK; continue; }
                m |= 1 << v0;
            }
            /* second cell */
            if ((m >> v1) & 1) {
                /* already seen */
            } else {
                int min_missing = 0;
                while ((m >> min_missing) & 1) ++min_missing;
                if (v1 != min_missing) { next_mask[mask][rc] = INVALID_MASK; continue; }
                m |= 1 << v1;
            }
            next_mask[mask][rc] = (unsigned char)m;
        }
    }

    /* ---- allowed_codes table ---- */
    for (int p1 = 0; p1 < 16; ++p1) {
        int p0 = row_v0(p1), p1v = row_v1(p1);
        for (int p2 = 0; p2 <= 16; ++p2) {
            uint8_t *codes = allowed_codes[p1][p2];
            uint8_t cnt = 0;
            bool prev2_exists = (p2 != NULL_ROW);
            int pv0 = prev2_exists ? row_v0(p2) : -1;
            int pv1 = prev2_exists ? row_v1(p2) : -1;
            for (int rc = 0; rc < 16; ++rc) {
                int c0 = row_v0(rc), c1 = row_v1(rc);
                bool ok = true;
                if (p0 == p1v) {               /* horizontal equal in row i‑1 */
                    /* column 0 */
                    if (!prev2_exists) {
                        if (c0 != p0) ok = false;
                    } else {
                        bool above = (pv0 == p0);
                        bool below = (c0 == p0);
                        if (!(above || below)) ok = false;
                    }
                    /* column 1 */
                    if (ok) {
                        if (!prev2_exists) {
                            if (c1 != p1v) ok = false;
                        } else {
                            bool above = (pv1 == p1v);
                            bool below = (c1 == p1v);
                            if (!(above || below)) ok = false;
                        }
                    }
                }
                if (ok) {
                    codes[cnt++] = (uint8_t)rc;
                }
            }
            allowed_cnt[p1][p2] = cnt;
        }
    }
}

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but safe */

    prepare_tables();

    /* DP arrays: dp[prev1][prev2][mask] */
    static seq_t dp[16][17][16];
    static seq_t dp_next[16][17][16];
    /* Initialise all to 0 */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 17; ++j)
            for (int k = 0; k < 16; ++k)
                dp[i][j][k] = 0;

    /* ----- initialise first row ----- */
    for (int rc = 0; rc < 16; ++rc) {
        unsigned char newm = next_mask[0][rc];   /* empty mask=0 */
        if (newm == INVALID_MASK) continue;
        dp[rc][NULL_ROW][newm] = (seq_t)1;
    }

    /* ----- process remaining rows ----- */
    for (idx_t row = 1; row < n; ++row) {
        /* zero dp_next */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 17; ++j)
                for (int k = 0; k < 16; ++k)
                    dp_next[i][j][k] = 0;

        for (int prev1 = 0; prev1 < 16; ++prev1) {
            int p0 = row_v0(prev1), p1v = row_v1(prev1);
            for (int prev2 = 0; prev2 < 17; ++prev2) {
                bool prev2_exists = (prev2 != NULL_ROW);
                int pv0 = prev2_exists ? row_v0(prev2) : -1;
                int pv1 = prev2_exists ? row_v1(prev2) : -1;
                for (int mask = 0; mask < 16; ++mask) {
                    seq_t cnt = dp[prev1][prev2][mask];
                    if (!cnt) continue;

                    uint8_t *carr = allowed_codes[prev1][prev2];
                    uint8_t csz = allowed_cnt[prev1][prev2];
                    for (uint8_t ci = 0; ci < csz; ++ci) {
                        int rc = carr[ci];
                        unsigned char nmask = next_mask[mask][rc];
                        if (nmask == INVALID_MASK) continue;
                        dp_next[rc][prev1][nmask] += cnt;
                    }
                }
            }
        }
        /* swap */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 17; ++j)
                for (int k = 0; k < 16; ++k)
                    dp[i][j][k] = dp_next[i][j][k];
    }

    /* ----- final row check ----- */
    seq_t result = 0;
    for (int last = 0; last < 16; ++last) {
        int l0 = row_v0(last), l1 = row_v1(last);
        bool last_equal = (l0 == l1);
        for (int prev = 0; prev < 17; ++prev) {
            bool prev_exists = (prev != NULL_ROW);
            for (int mask = 0; mask < 16; ++mask) {
                seq_t cnt = dp[last][prev][mask];
                if (!cnt) continue;

                bool ok = true;
                if (last_equal) {
                    if (!prev_exists) {
                        ok = false;       /* no vertical neighbour to match */
                    } else {
                        int p0 = row_v0(prev), p1 = row_v1(prev);
                        if (p0 != l0 || p1 != l1) ok = false;
                    }
                }
                if (ok) result += cnt;
            }
        }
    }
    return result;
}
