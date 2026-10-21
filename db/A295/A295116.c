/*
 * A295116  Number of nX4 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 1 or 4 1s.
 * https://oeis.org/A295116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (empirical):
   a(n) = a(n-1)+5*a(n-2)+22*a(n-3)+43*a(n-4)+52*a(n-5)+35*a(n-6)
          +35*a(n-7)+21*a(n-8)+14*a(n-9)+6*a(n-10)+9*a(n-11)
          -7*a(n-12)-a(n-14)   (valid for all n ≥ 1)
   This function computes a(n) by dynamic programming over rows
   of the n×4 0/1 array, ensuring each 1 cell has a total
   horizontal‑plus‑vertical neighbor count of 1 or 4.  */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0U) return 0;

    /* Precompute horizontal degrees for each mask and column. */
    static int horiz_deg[16][4];
    static bool initialized = false;
    if (!initialized) {
        for (int mask = 0; mask < 16; ++mask) {
            for (int j = 0; j < 4; ++j) {
                int d = 0;
                if (j > 0 && ((mask >> (j - 1)) & 1)) ++d;
                if (j < 3 && ((mask >> (j + 1)) & 1)) ++d;
                horiz_deg[mask][j] = d;
            }
        }
        initialized = true;
    }

    /* DP states: index = (prev_mask << 4) | cur_mask (0..255). */
    seq_t dp[256] = {0};
    seq_t newdp[256];
    for (int cur = 0; cur < 16; ++cur) {
        dp[cur] = 1;                 /* prev_mask = 0 (dummy row before row 1) */
    }

    /* Add rows 2..n. */
    for (idx_t row = 1; row < n; ++row) {
        for (int i = 0; i < 256; ++i) newdp[i] = 0;

        for (int idx = 0; idx < 256; ++idx) {
            seq_t val = dp[idx];
            if (val == 0) continue;
            int prev = idx >> 4;
            int cur  = idx & 15;

            for (int next = 0; next < 16; ++next) {
                bool ok = true;
                for (int j = 0; j < 4 && ok; ++j) {
                    if ((cur >> j) & 1) {
                        int horiz = horiz_deg[cur][j];
                        int vert  = ((prev >> j) & 1) + ((next >> j) & 1);
                        int tot   = horiz + vert;
                        if (tot != 1 && tot != 4) ok = false;
                    }
                }
                if (!ok) continue;
                int newidx = (cur << 4) | next;
                newdp[newidx] += val;
            }
        }

        for (int i = 0; i < 256; ++i) dp[i] = newdp[i];
    }

    /* Final check for the last row (next row is dummy 0). */
    seq_t total = 0;
    for (int idx = 0; idx < 256; ++idx) {
        seq_t val = dp[idx];
        if (val == 0) continue;
        int prev = idx >> 4;
        int cur  = idx & 15;

        bool ok = true;
        for (int j = 0; j < 4 && ok; ++j) {
            if ((cur >> j) & 1) {
                int horiz = horiz_deg[cur][j];
                int vert  = ((prev >> j) & 1);  /* next is 0 */
                int tot   = horiz + vert;
                if (tot != 1 && tot != 4) ok = false;
            }
        }
        if (!ok) continue;
        total += val;
    }

    return total;
}
