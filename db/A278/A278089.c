/*
 * A278089  Number of n X 3 0..1 arrays with every element both equal and not
 *          equal to some elements at offset (-1,0) (-1,1) (0,-1) (0,1) or
 *          (1,0), with upper left element zero.
 * https://oeis.org/A278089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 binary matrices with top-left cell 0 such
 *            that every cell has at least one equal and one unequal neighbor
 *            among the offsets (-1,0),(-1,1),(0,-1),(0,1),(1,0). Computed by
 *            dynamic programming over rows, storing for each previous row the
 *            pattern and pending requirements for a do...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>   /* memset, memcpy */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, but safe */

    static seq_t dp[512];
    static seq_t newdp[512];

    /* initialise zero */
    memset(dp, 0, sizeof(dp));

    /* row 1: upper‑left element (column 0) must be 0 */
    for (int p = 0; p < 8; ++p) {
        if ((p & 1) != 0) continue;   /* bit0 must be 0 */

        /* compute pending need for each of the 3 columns in this row */
        int status = 0;
        for (int j = 0; j < 3; ++j) {
            int same_sat = 0;
            int diff_sat = 0;

            /* left neighbour */
            if (j > 0) {
                int bj   = (p >> j) & 1;
                int bjm1 = (p >> (j - 1)) & 1;
                if (bj == bjm1) same_sat = 1; else diff_sat = 1;
            }
            /* right neighbour */
            if (j < 2) {
                int bj    = (p >> j) & 1;
                int bjp1  = (p >> (j + 1)) & 1;
                if (bj == bjp1) same_sat = 1; else diff_sat = 1;
            }

            int need_same = !same_sat;
            int need_diff = !diff_sat;
            status |= (need_same << (2 * j));
            status |= (need_diff << (2 * j + 1));
        }
        dp[p * 64 + status] += 1;
    }

    /* if only a single row, only patterns with no pending down requirements are valid */
    if (n == 1) {
        seq_t res = 0;
        for (int p = 0; p < 8; ++p) res += dp[p * 64];
        return res;
    }

    /* process the remaining rows */
    for (idx_t r = 2; r <= n; ++r) {
        memset(newdp, 0, sizeof(newdp));

        for (int s = 0; s < 512; ++s) {
            seq_t val = dp[s];
            if (val == 0) continue;

            int prev_p = s >> 6;           /* pattern of previous row (0..7) */
            int st     = s & 63;           /* pending requirements bits */

            int prev_bits[3];
            prev_bits[0] = (prev_p >> 0) & 1;
            prev_bits[1] = (prev_p >> 1) & 1;
            prev_bits[2] = (prev_p >> 2) & 1;

            int need_same_prev[3];
            int need_diff_prev[3];
            need_same_prev[0] = (st >> 0)  & 1;
            need_same_prev[1] = (st >> 2)  & 1;
            need_same_prev[2] = (st >> 4)  & 1;
            need_diff_prev[0] = (st >> 1)  & 1;
            need_diff_prev[1] = (st >> 3)  & 1;
            need_diff_prev[2] = (st >> 5)  & 1;

            for (int cur = 0; cur < 8; ++cur) {
                /* check whether next row satisfies the pending needs of the previous row */
                int ok = 1;
                for (int j = 0; j < 3; ++j) {
                    int up_bit  = prev_bits[j];
                    int cur_bit = (cur >> j) & 1;
                    if (need_same_prev[j] && cur_bit != up_bit) { ok = 0; break; }
                    if (need_diff_prev[j] && cur_bit == up_bit) { ok = 0; break; }
                }
                if (!ok) continue;

                /* compute new pending requirements for this row */
                int new_status = 0;
                for (int j = 0; j < 3; ++j) {
                    int same_sat = 0;
                    int diff_sat = 0;
                    int cur_bit  = (cur >> j) & 1;

                    /* left neighbour */
                    if (j > 0) {
                        int left_bit = (cur >> (j - 1)) & 1;
                        if (cur_bit == left_bit) same_sat = 1; else diff_sat = 1;
                    }
                    /* right neighbour */
                    if (j < 2) {
                        int right_bit = (cur >> (j + 1)) & 1;
                        if (cur_bit == right_bit) same_sat = 1; else diff_sat = 1;
                    }
                    /* up neighbour */
                    int up_bit = prev_bits[j];
                    if (cur_bit == up_bit) same_sat = 1; else diff_sat = 1;
                    /* up‑right neighbour */
                    if (j < 2) {
                        int up_r_bit = prev_bits[j + 1];
                        if (cur_bit == up_r_bit) same_sat = 1; else diff_sat = 1;
                    }

                    int need_same_new = !same_sat;
                    int need_diff_new = !diff_sat;
                    new_status |= (need_same_new << (2 * j));
                    new_status |= (need_diff_new << (2 * j + 1));
                }
                newdp[cur * 64 + new_status] += val;
            }
        }
        memcpy(dp, newdp, sizeof(dp));
    }

    /* sum states of the last row that have no pending requirements */
    seq_t res = 0;
    for (int p = 0; p < 8; ++p) res += dp[p * 64];
    return res;
}
