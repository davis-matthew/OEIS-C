/*
 * A301538  Number of n X 5 0..1 arrays with every element equal to 0, 1 or 3
 *          horizontally or vertically adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays with upper‑left zero such
 *            that for every cell the number of horizontally or vertically
 *            adjacent cells with the same value equals 0, 1, or 3. The
 *            function enumerates all such arrays using dynamic programming
 *            over rows; the state keeps the previous two rows and ch...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define MASKS 32               /* 5 bits → 0..31 */
#define SENTINEL MASKS         /* index used for “no up row” */
#define STATE_ROWS 33          /* 0..31 masks + sentinel */

/* Precomputed transition masks: for each (pp, p) we store a bitmask
   indicating all possible next rows c that keep the validity of row p. */
static uint32_t compat_mask[STATE_ROWS][MASKS];

/* Helper to precompute compat_mask once (no side effects). */
static void init_compat_mask(void) {
    static bool done = false;
    if (done) return;
    done = true;

    for (int pp = 0; pp <= SENTINEL; ++pp) {
        for (int p = 0; p < MASKS; ++p) {
            uint32_t m = 0;
            for (int c = 0; c < MASKS; ++c) {
                bool ok = true;
                for (int j = 0; j < 5; ++j) {
                    int val = (p >> j) & 1;

                    int left_same  = (j > 0   && ((p >> (j-1)) & 1) == val) ? 1 : 0;
                    int right_same = (j < 4   && ((p >> (j+1)) & 1) == val) ? 1 : 0;
                    int up_same    = (pp == SENTINEL) ? 0
                                      : (((pp >> j) & 1) == val) ? 1 : 0;
                    int down_same  = ((c >> j) & 1) == val ? 1 : 0;

                    int total = left_same + right_same + up_same + down_same;
                    if (total != 0 && total != 1 && total != 3) {
                        ok = false;
                        break;
                    }
                }
                if (ok) m |= (1u << c);
            }
            compat_mask[pp][p] = m;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Handle offsets below 1 – result is 0 (does not matter). */
    if (n < 1) return 0;

    init_compat_mask();

    /* DP arrays: dp[prevprev][prev] = count */
    static seq_t dp[STATE_ROWS][MASKS];
    static seq_t newdp[STATE_ROWS][MASKS];

    /* Zero the arrays */
    for (int i = 0; i < STATE_ROWS; ++i)
        for (int j = 0; j < MASKS; ++j)
            dp[i][j] = 0;

    /* Initial row (row 0). Upper‑left element must be 0. */
    for (int m = 0; m < MASKS; ++m) {
        if ((m & 1) == 0) {                 /* bit 0 is the leftmost column */
            dp[SENTINEL][m] = 1;
        }
    }

    /* Process rows 1 … n‑1 */
    for (idx_t row = 1; row < n; ++row) {
        /* Zero newdp */
        for (int i = 0; i < STATE_ROWS; ++i)
            for (int j = 0; j < MASKS; ++j)
                newdp[i][j] = 0;

        for (int pp = 0; pp <= SENTINEL; ++pp) {
            for (int p = 0; p < MASKS; ++p) {
                seq_t curcnt = dp[pp][p];
                if (curcnt == 0) continue;

                uint32_t maskc = compat_mask[pp][p];
                /* iterate over all c where bit is set */
                for (int c = 0; c < MASKS; ++c) {
                    if (maskc & (1u << c)) {
                        newdp[p][c] += curcnt;
                    }
                }
            }
        }
        /* Copy newdp into dp for next iteration */
        for (int i = 0; i < STATE_ROWS; ++i)
            for (int j = 0; j < MASKS; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* Finalize the last row (row n‑1): no down neighbor. */
    seq_t ans = 0;
    for (int pp = 0; pp <= SENTINEL; ++pp) {
        for (int p = 0; p < MASKS; ++p) {
            seq_t cnt = dp[pp][p];
            if (cnt == 0) continue;

            bool ok = true;
            for (int j = 0; j < 5; ++j) {
                int val = (p >> j) & 1;
                int left_same  = (j > 0   && ((p >> (j-1)) & 1) == val) ? 1 : 0;
                int right_same = (j < 4   && ((p >> (j+1)) & 1) == val) ? 1 : 0;
                int up_same    = (pp == SENTINEL) ? 0
                                   : (((pp >> j) & 1) == val) ? 1 : 0;
                int total = left_same + right_same + up_same;   /* down none */

                if (total != 0 && total != 1 && total != 3) {
                    ok = false;
                    break;
                }
            }
            if (ok) ans += cnt;
        }
    }

    return ans;
}
