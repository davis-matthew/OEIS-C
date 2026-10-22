/*
 * A394505  Number of ways to place non-attacking knights on a 5 X n
 *          chessboard.
 * https://oeis.org/A394505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the number of independent vertex sets (including the
 *            empty set) in the 5×n knight graph. Computed by DP over states
 *            of the last two columns
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External types provided by the build. */
#include <stdint.h>
#include <stdbool.h>

/* Counts valid knight placements column‑by‑column.  The state is a 5‑bit mask
   of occupied rows in a column.  For each new column we must ensure that
   no knight attacks cells in the previous column (±2 rows) or in the column
   two steps back (±1 row).  The DP keeps all pairs (mask_{i‑2}, mask_{i‑1})
   and transitions to (mask_{i‑1}, mask_i).  At the end the sum of all DP
   states is the required number of independent sets. */
seq_t a(idx_t n)
{
    /* helper type for small integer masks */
    typedef unsigned char u8;
    /* Pre‑computed transition lists: for each previous‑prev and previous
       masks we store all masks that can be placed in the next column.  The
       number of such masks is small (≤32). */
    static bool built = false;
    static u8  allowed_count[32][32];
    static u8  allowed_masks[32][32][32];
    static uint32_t forb_shift2[32], forb_shift1[32];
    if (!built) {
        /* forbidden patterns due to a previous column: ±2 rows */
        for (int a = 0; a < 32; ++a) {
            uint32_t up  = (a << 2) & 0x1F;
            uint32_t down = (a >> 2) ;
            forb_shift2[a] = (up | down) & 0x1F;
            /* forbidden due to column two steps back: ±1 row */
            up   = (a << 1) & 0x1F;
            down = (a >> 1);
            forb_shift1[a] = (up | down) & 0x1F;
        }
        /* build transition lists */
        for (int pp = 0; pp < 32; ++pp) {
            for (int p = 0; p < 32; ++p) {
                u8 cnt = 0;
                for (int m = 0; m < 32; ++m) {
                    if ( (m & forb_shift2[p]) == 0 && (m & forb_shift1[pp]) == 0 ) {
                        allowed_masks[pp][p][cnt++] = (u8)m;
                    }
                }
                allowed_count[pp][p] = cnt;
            }
        }
        built = true;
    }

    /* DP tables: dp[prev_prev][prev] holds counts for the two most recent
       columns.  We only need two such tables. */
    seq_t dp[32][32];
    seq_t nxt[32][32];
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = 0;
    dp[0][0] = 1; /* start with two imaginary empty columns */

    /* process each of the n columns */
    for (idx_t col = 0; col < n; ++col) {
        /* clear next table */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                nxt[i][j] = 0;

        for (int pp = 0; pp < 32; ++pp) {      /* mask of column i-2 */
            for (int p = 0; p < 32; ++p) {      /* mask of column i-1 */
                seq_t cnt = dp[pp][p];
                if (cnt == 0) continue;
                u8 c = allowed_count[pp][p];
                for (u8 k = 0; k < c; ++k) {
                    u8 m = allowed_masks[pp][p][k];
                    nxt[p][m] += cnt;
                }
            }
        }
        /* move nxt to dp */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = nxt[i][j];
    }

    /* sum all final states */
    seq_t ans = 0;
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            ans += dp[i][j];
    return ans;
}
