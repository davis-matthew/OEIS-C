/*
 * A295712  Number of nX4 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 0, 1, 2 or 4 1s.
 * https://oeis.org/A295712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   count of n×4 0..1 arrays where every 1 has 0,1,2 or 4
 *            horizontally/vertically * adjacent 1s. Computed by dynamic
 *            programming over rows
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>

/* Pre‑computed horizontal neighbour counts for each 4‑bit pattern.  */
static unsigned char hcnt[16][4];
static bool hcnt_ready = false;

static void init_hcnt(void)
{
    if (hcnt_ready) return;
    for (int pat = 0; pat < 16; ++pat) {
        for (int j = 0; j < 4; ++j) {
            unsigned char cnt = 0;
            if (j > 0  && ((pat >> (j - 1)) & 1)) ++cnt;
            if (j < 3  && ((pat >> (j + 1)) & 1)) ++cnt;
            hcnt[pat][j] = cnt;
        }
    }
    hcnt_ready = true;
}

/* Check that the middle row `prev` does not contain a 1 with exactly
 * 3 adjacent 1s (horizontal + vertical above/below).  */
static inline bool check_row(int prevPrev, int prev, int cur)
{
    for (int j = 0; j < 4; ++j) {
        if (!((prev >> j) & 1)) continue;        /* only look at 1s           */
        int total = hcnt[prev][j];                /* horizontal neighbours    */
        if ((prevPrev >> j) & 1) ++total;         /* vertical above neighbour */
        if ((cur >> j) & 1)   ++total;            /* vertical below neighbour */
        if (total == 3) return false;             /* forbidden configuration  */
    }
    return true;
}

/* Check that the last row `cur` (with previous row `prev`) does not
 * contain a 1 with exactly 3 adjacent 1s.  Only the horizontal
 * neighbours and the single vertical neighbour above are considered. */
static inline bool check_last_row(int prev, int cur)
{
    for (int j = 0; j < 4; ++j) {
        if (!((cur >> j) & 1)) continue;
        int total = hcnt[cur][j];                 /* horizontal neighbours   */
        if ((prev >> j) & 1) ++total;              /* vertical above neighbour */
        if (total == 3) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for index 0, but safe return */
    /* Special case n == 1: all 16 patterns are valid. */
    if (n == 1) return 16;

    init_hcnt();

    /* dp[a*16 + b] holds number of ways to reach a state where
     * the last two rows are: previous previous = a, previous = b.   */
    static seq_t dp[256], ndp[256];
    memset(dp, 0, sizeof(dp));

    /* Initialise with first row (prevPrev = 0). */
    for (int pat = 0; pat < 16; ++pat) dp[pat] = 1;

    /* Process remaining rows 2 … n. */
    for (idx_t row = 2; row <= n; ++row) {
        memset(ndp, 0, sizeof(ndp));

        for (int a = 0; a < 16; ++a)           /* prevPrev */
            for (int b = 0; b < 16; ++b) {      /* prev      */
                seq_t ways = dp[a * 16 + b];
                if (!ways) continue;
                for (int c = 0; c < 16; ++c) {  /* cur       */
                    if (!check_row(a, b, c)) continue;
                    ndp[b * 16 + c] += ways;
                }
            }
        memcpy(dp, ndp, sizeof(dp));
    }

    /* Final validation: last row has no below neighbour. */
    seq_t result = 0;
    for (int a = 0; a < 16; ++a)          /* prev    */
        for (int b = 0; b < 16; ++b) {     /* cur (last) */
            if (!check_last_row(a, b)) continue;
            result += dp[a * 16 + b];
        }

    return result;
}
