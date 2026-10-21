/*
 * A297730  Number of nX5 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 2 or 4 neighboring 1s.
 * https://oeis.org/A297730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays in which every 1 has exactly
 *            2 or 4 horizontally, diagonally or anti‑diagonally adjacent 1s.
 *            This is solved by a transfer‑matrix DP over rows
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* helper: check that all 1‑cells in 'prev' have degree 2 or 4
   given the rows above (pprev) and below (curr). */
static bool row_valid(unsigned int pprev, unsigned int prev, unsigned int curr)
{
    for (int j = 0; j < 5; ++j) {
        if ((prev >> j) & 1u) {
            int cnt = 0;
            /* horizontal neighbors */
            if (j > 0 && (prev >> (j - 1)) & 1u) cnt++;
            if (j < 4 && (prev >> (j + 1)) & 1u) cnt++;
            /* diagonal up */
            if (j > 0 && (pprev >> (j - 1)) & 1u) cnt++;
            if (j < 4 && (pprev >> (j + 1)) & 1u) cnt++;
            /* diagonal down */
            if (j > 0 && (curr >> (j - 1)) & 1u) cnt++;
            if (j < 4 && (curr >> (j + 1)) & 1u) cnt++;
            if (cnt != 2 && cnt != 4)
                return false;
        }
    }
    return true;
}

/* Pre‑computed table: for each pair of previous rows (pprev,prev)
   a 32‑bit mask whose k‑th bit is 1 iff the third row 'k' makes
   'prev' valid. */
static uint32_t valid_mask[32][32];
static bool valid_mask_init = false;

static void init_valid_mask(void)
{
    if (valid_mask_init) return;
    for (unsigned int pprev = 0; pprev < 32; ++pprev) {
        for (unsigned int prev  = 0; prev  < 32; ++prev) {
            uint32_t mask = 0;
            for (unsigned int curr = 0; curr < 32; ++curr)
                if (row_valid(pprev, prev, curr))
                    mask |= 1u << curr;
            valid_mask[pprev][prev] = mask;
        }
    }
    valid_mask_init = true;
}

seq_t a(idx_t n)
{
    /* DP arrays: dp[pprev][prev] counts ways ending with these two rows.
       newdp[prev][curr] is the next state after adding a row 'curr'. */
    static seq_t dp[32][32], newdp[32][32];
    memset(dp, 0, sizeof(dp));
    dp[0][0] = 1;                 /* before any row: two zero rows */

    init_valid_mask();

    for (idx_t row = 1; row <= n; ++row) {
        memset(newdp, 0, sizeof(newdp));

        for (unsigned int pprev = 0; pprev < 32; ++pprev) {
            for (unsigned int prev = 0; prev < 32; ++prev) {
                seq_t ways = dp[pprev][prev];
                if (!ways) continue;

                uint32_t mask = valid_mask[pprev][prev];
                while (mask) {
                    int curr = __builtin_ctz(mask);
                    mask &= mask - 1;
                    newdp[prev][curr] += ways;
                }
            }
        }
        /* swap dp and newdp */
        for (int i = 0; i < 32; ++i)
            memcpy(dp[i], newdp[i], sizeof(seq_t) * 32);
    }

    /* After processing n rows we must validate the last row
       with the row above and no row below (curr = 0). */
    seq_t ans = 0;
    for (unsigned int pprev = 0; pprev < 32; ++pprev) {
        for (unsigned int last  = 0; last < 32; ++last) {
            if (dp[pprev][last] && row_valid(pprev, last, 0))
                ans += dp[pprev][last];
        }
    }
    return ans;
}
