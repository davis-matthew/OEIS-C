/*
 * A251114  Number of (n+1) X (1+1) 0..3 arrays with nondecreasing maximum of
 *          every two consecutive values in every row and column.
 * https://oeis.org/A251114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 22, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dynamic programming over rows.
 * For an (n+1)×2 array of symbols {0,1,2,3} we require that
 * for each column the sequence of maxima of consecutive entries
 * (max(a[i-1][j],a[i][j])) is nondecreasing.
 * This DP keeps the last row values and the previous vertical maxima.
 * State (l0,l1,m0,m1) is encoded in 8 bits:
 *   l0 = bit 0..1, l1 = bit 2..3, m0 = bit 4..5, m1 = bit 6..7.
 * Transitions generate the next row and update the maxima,
 * keeping the nondecreasing property.  Complexity: O(n·256·16). */

#include <stdint.h>
#include <string.h>

#define STATE_CNT 256  /* 4⁴ possible states */

static seq_t dp[STATE_CNT];
static seq_t ndp[STATE_CNT];

seq_t a(idx_t n)
{
    /* n >= 1 : total rows = n+1 */
    memset(dp, 0, sizeof(dp));

    /* initialise with the first row (row 0) */
    for (int a0 = 0; a0 < 4; ++a0)
        for (int a1 = 0; a1 < 4; ++a1)
        {
            int idx = (a0) | (a1 << 2) | (0 << 4) | (0 << 6); /* m0=m1=0 */
            dp[idx] += 1;
        }

    for (idx_t step = 1; step <= n; ++step)   /* add rows 1..n */
    {
        memset(ndp, 0, sizeof(ndp));

        for (int s = 0; s < STATE_CNT; ++s)
        {
            seq_t cur = dp[s];
            if (cur == 0) continue;

            int l0 =  s        & 3;
            int l1 = (s >> 2)  & 3;
            int pm0 = (s >> 4) & 3;
            int pm1 = (s >> 6) & 3;

            for (int b0 = 0; b0 < 4; ++b0)
                for (int b1 = 0; b1 < 4; ++b1)
                {
                    int m0 = l0 > b0 ? l0 : b0;
                    int m1 = l1 > b1 ? l1 : b1;
                    if (m0 < pm0 || m1 < pm1) continue;

                    int idx = (b0) | (b1 << 2) | (m0 << 4) | (m1 << 6);
                    ndp[idx] += cur;
                }
        }

        memcpy(dp, ndp, sizeof(dp));
    }

    seq_t ans = 0;
    for (int s = 0; s < STATE_CNT; ++s) ans += dp[s];
    return ans;
}
