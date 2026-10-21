/*
 * A299751  Number of nX6 0..1 arrays with every element equal to 0, 1, 2, 3,
 *          5, 6 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A299751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 binary arrays (0/1) with the restriction
 *            that for each element the number of equal values among its
 *            king-move adjacent cells (including diagonals) is not 4 or 7,
 *            and the upper-left element is zero
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses dynamic programming over rows with a
   state that consists of the last two rows.  The number of states
   is 2^12 = 4096.  For each state we precompute a 64‑bit mask of
   all next rows that keep the middle row valid.  The transition
   graph allows easy iteration in O(n). */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static bool valid_row(uint8_t above, bool has_above,
                      uint8_t curr,
                      uint8_t below, bool has_below)
{
    for (int c = 0; c < 6; ++c) {
        int val = (curr >> c) & 1;
        int cnt = 0;
        for (int dy = -1; dy <= 1; ++dy) {
            if (dy == 0) {
                /* same row */
                for (int dx = -1; dx <= 1; ++dx) {
                    if (dx == 0) continue;
                    int nc = c + dx;
                    if (nc < 0 || nc >= 6) continue;
                    int nval = (curr >> nc) & 1;
                    if (nval == val) ++cnt;
                }
            } else if (dy == -1) {
                /* above */
                if (!has_above) continue;
                for (int dx = -1; dx <= 1; ++dx) {
                    int nc = c + dx;
                    if (nc < 0 || nc >= 6) continue;
                    int nval = (above >> nc) & 1;
                    if (nval == val) ++cnt;
                }
            } else { /* dy == 1 , below */
                if (!has_below) continue;
                for (int dx = -1; dx <= 1; ++dx) {
                    int nc = c + dx;
                    if (nc < 0 || nc >= 6) continue;
                    int nval = (below >> nc) & 1;
                    if (nval == val) ++cnt;
                }
            }
        }
        if (cnt == 4 || cnt == 7) return false;
    }
    return true;
}

static void init_transition_masks(uint64_t *mask)
{
    for (int pp = 0; pp < 64; ++pp) {
        for (int p = 0; p < 64; ++p) {
            int state = (pp << 6) | p;
            uint64_t bits = 0;
            for (int nb = 0; nb < 64; ++nb) {
                if (valid_row((uint8_t)pp, true,
                              (uint8_t)p,
                              (uint8_t)nb, true)) {
                    bits |= ((uint64_t)1 << nb);
                }
            }
            mask[state] = bits;
        }
    }
}

static const uint64_t *transitions_mask(void)
{
    static bool initialized = false;
    static uint64_t mask[4096];
    if (!initialized) {
        init_transition_masks(mask);
        initialized = true;
    }
    return mask;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined, return 0 */
    if (n == 1) return (seq_t)32;       /* only first row, all 6-bit patterns with bit0=0 */

    const uint64_t *TM = transitions_mask();

    /* DP arrays: dp holds paths ending with rows (prevprev, prev) */
    static seq_t dp[4096];
    static seq_t ndp[4096];
    memset(dp, 0, sizeof(dp));

    /* Initial step: choose row1 with bit0==0, and any row2 */
    for (int r1 = 0; r1 < 64; ++r1) {
        if (r1 & 1) continue;                 /* upper-left must be 0 */
        for (int r2 = 0; r2 < 64; ++r2) {
            if (!valid_row(0, false, (uint8_t)r1, (uint8_t)r2, true))
                continue;
            int idx = (r1 << 6) | r2;
            dp[idx] += (seq_t)1;
        }
    }

    /* Process rows 3 .. n */
    for (idx_t row = 3; row <= n; ++row) {
        memset(ndp, 0, sizeof(ndp));
        for (int cur = 0; cur < 4096; ++cur) {
            seq_t cnt = dp[cur];
            if (cnt == (seq_t)0) continue;
            int p = cur & 63;                 /* previous row */
            uint64_t bits = TM[cur];
            while (bits) {
                int nb = __builtin_ctzll(bits);
                bits &= bits - 1;             /* clear lowest set bit */
                int nidx = (p << 6) | nb;
                ndp[nidx] += cnt;
            }
        }
        memcpy(dp, ndp, sizeof(dp));
    }

    /* Finalization: verify last row (prev) with no below neighbors */
    seq_t result = 0;
    for (int cur = 0; cur < 4096; ++cur) {
        seq_t cnt = dp[cur];
        if (cnt == (seq_t)0) continue;
        int pp = cur >> 6;
        int p  = cur & 63;
        if (valid_row((uint8_t)pp, true,
                      (uint8_t)p,
                      0, false)) {
            result += cnt;
        }
    }
    return result;
}
