/*
 * A302327  Number of 6Xn 0..1 arrays with every element equal to 0, 1, 2 or 4
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 6×n binary arrays in which, for every element,
 *            the * number of horizontally, diagonally or antidiagonally
 *            adjacent elements * having the same value is 0, 1, 2 or 4 (i.e.
 *            it must not be 3, 5 or 6); * the upper-left element is fixed to
 *            zero. The function uses a transfer‑matrix * DP...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Helpers:   col_valid_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "../utils/col_valid_bits.h"

/*------------------------------------------------------------*/
/*  Auxiliary tables (built once)                            */
/*------------------------------------------------------------*/
static bool start_ok[64][64];
static bool end_ok[64][64];
static uint64_t allowed_mid_mask[64][64];
static uint64_t next_mask[64][64];
static bool tables_ready = false;

/*------------------------------------------------------------*/
/*  For a given column (mid) check validity with its left and
 *  right neighbour columns.  The column is valid if, for every
 *  cell (bit), the count of adjacent cells (horizontally,
 *  diagonally or antidiagonally) having the same value is
 *  NOT 3, 5 or 6.                                            */

/*------------------------------------------------------------*/
/*  Build the lookup tables once.                            */
/*------------------------------------------------------------*/
static void build_tables(void)
{
    if (tables_ready) return;

    for (int left = 0; left < 64; ++left) {
        for (int right = 0; right < 64; ++right) {
            /* first column has no left neighbour */
            start_ok[left][right] =
                col_valid_bits(left, 0, right, 0, 1);

            /* last column has no right neighbour */
            end_ok[left][right] =
                col_valid_bits(right, left, 0, 1, 0);

            uint64_t mask = 0;
            for (int mid = 0; mid < 64; ++mid)
                if (col_valid_bits(mid, left, right, 1, 1))
                    mask |= (1ULL << mid);
            allowed_mid_mask[left][right] = mask;
        }
    }

    /* next_mask[prev][curr] lists all next columns that are
       compatible with the pair (prev,curr).                 */
    for (int prev = 0; prev < 64; ++prev)
        for (int curr = 0; curr < 64; ++curr) {
            uint64_t m = 0;
            for (int next = 0; next < 64; ++next)
                if ((allowed_mid_mask[prev][next] >> curr) & 1ULL)
                    m |= (1ULL << next);
            next_mask[prev][curr] = m;
        }

    tables_ready = true;
}

/*------------------------------------------------------------*/
/*  Main sequence routine                                    */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    build_tables();

    if (n == 1U)
        return (seq_t)32;      /* 1<<5 is 32, but 32 is simpler */

    /* DP states: combination of two consecutive columns (prev,curr).  6 bits
     * per column → 12 bits total, 4096 possible states.                */
    static seq_t dp_prev[1 << 12];
    static seq_t dp_curr[1 << 12];
    memset(dp_prev, 0, sizeof(dp_prev));
    memset(dp_curr, 0, sizeof(dp_curr));

    /* Initialize all pairs of columns that satisfy the start condition. */
    for (int col1 = 0; col1 < 64; ++col1) {
        if (col1 & 1) continue;              /* upper left must be 0 */
        for (int col2 = 0; col2 < 64; ++col2) {
            if (!start_ok[col1][col2]) continue;
            int idx = (col1 << 6) | col2;
            dp_prev[idx] = (seq_t)1;
        }
    }

    if (n == 2U) {
        seq_t ans = 0;
        for (int idx = 0; idx < (1 << 12); ++idx) {
            if (dp_prev[idx] == 0) continue;
            int col1 = idx >> 6;
            int col2 = idx & 63;
            if (end_ok[col1][col2]) ans += dp_prev[idx];
        }
        return ans;
    }

    /* For n >= 3, extend the matrix column by column. */
    for (idx_t step = 3; step <= n; ++step) {
        memset(dp_curr, 0, sizeof(dp_curr));
        for (int idx = 0; idx < (1 << 12); ++idx) {
            seq_t val = dp_prev[idx];
            if (val == 0) continue;
            int prev = idx >> 6;
            int curr = idx & 63;
            uint64_t m = next_mask[prev][curr];
            while (m) {
                uint64_t low = m & -m;
                int next = __builtin_ctzll(m);
                int nxt_idx = (curr << 6) | next;
                dp_curr[nxt_idx] += val;
                m ^= low;
            }
        }
        memcpy(dp_prev, dp_curr, sizeof(dp_prev));
    }

    /* Final boundary check for last column. */
    seq_t ans = 0;
    for (int idx = 0; idx < (1 << 12); ++idx) {
        seq_t val = dp_prev[idx];
        if (val == 0) continue;
        int col_n_1 = idx >> 6;
        int col_n   = idx & 63;
        if (end_ok[col_n_1][col_n]) ans += val;
    }
    return ans;
}
