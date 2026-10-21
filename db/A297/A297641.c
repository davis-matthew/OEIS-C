/*
 * A297641  Number of 5Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 0, 2 or 3 neighboring 1s.
 * https://oeis.org/A297641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Counts 5×n binary arrays in which every 1 has 0, 2, or 3 *
 *            horizontally or diagonally adjacent 1s. The count is computed by
 *            * a transfer‐matrix dynamic programming with 32×32 states
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   check_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/check_valid.h"

/* --------------------------------------------------------------- */
/*  validity table:  valid[left][mid][right] == true  iff  the
 *  middle column mid satisfies the degree condition when its left
 *  neighbor is the pattern 'left' and its right neighbor is
 *  'right' (0 means no column).  The neighbor relation includes
 *  horizontal and both diagonal directions, but not vertical.   */
static unsigned char valid_triple[32][32][32];
static bool valid_precomputed = false;

static void precompute_valid(void)
{
    if (valid_precomputed) return;
    for (int left = 0; left < 32; ++left)
        for (int mid = 0; mid < 32; ++mid)
            for (int right = 0; right < 32; ++right)
                valid_triple[left][mid][right] =
                    check_valid(left, mid, right) ? 1 : 0;
    valid_precomputed = true;
}

/* --------------------------------------------------------------- */
/*  allow bitmask of next patterns that are compatible with a given
 *  pair (left, mid).  Precomputing helps speed up the DP. */
static uint32_t next_mask[32][32];
static bool next_mask_precomputed = false;

static void precompute_next_mask(void)
{
    if (next_mask_precomputed) return;
    for (int left = 0; left < 32; ++left)
        for (int mid = 0; mid < 32; ++mid) {
            uint32_t m = 0;
            for (int right = 0; right < 32; ++right)
                if (valid_triple[left][mid][right]) m |= (1u << right);
            next_mask[left][mid] = m;
        }
    next_mask_precomputed = true;
}

/* --------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* n >= 1 is guaranteed by the specification; handle n==0 trivially */
    if (n == 0u) return (seq_t)0;

    precompute_valid();
    precompute_next_mask();

    /* dp[prev][prevprev]  – number of ways ending with
     * column 'prev' and its left neighbour 'prevprev' */
    seq_t dp[32][32] = {0}, newdp[32][32] = {0};

    /* initialise: first column can be any pattern; left neighbour is 0 */
    for (int cur = 0; cur < 32; ++cur)
        dp[cur][0] = (seq_t)1;

    /* process columns 1 .. n-1 */
    for (idx_t step = 1; step < n; ++step) {
        /* zero newdp */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                newdp[i][j] = 0;

        for (int left = 0; left < 32; ++left)           /* prevprev   */
            for (int mid = 0; mid < 32; ++mid) {         /* prev       */
                seq_t val = dp[mid][left];
                if (!val) continue;
                uint32_t mask = next_mask[left][mid];
                while (mask) {
                    int right = __builtin_ctz(mask);
                    newdp[right][mid] += val;
                    mask &= mask - 1;
                }
            }

        /* copy newdp to dp for next iteration */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* compute the final answer: evaluate the last column */
    seq_t result = 0;
    if (n == 1u) {
        /* last column is also the first; left and right neighbours are 0 */
        for (int mask = 0; mask < 32; ++mask) {
            if (valid_triple[0][mask][0]) result += 1;
        }
    } else {
        for (int left = 0; left < 32; ++left)   /* prevprev  */
            for (int cur = 0; cur < 32; ++cur) { /* prev      */
                if (!dp[cur][left]) continue;
                if (valid_triple[left][cur][0]) result += dp[cur][left];
            }
    }

    return result;
}
