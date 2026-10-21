/*
 * A243638  Number of length n+2 0..7 arrays with no three unequal elements in
 *          a row and new values 0..7 introduced in 0..7 order.
 * https://oeis.org/A243638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   DP over last two values and the maximum value used. The state
 *            dp[x][y][m] counts arrays whose last two elements are x and y
 *            and whose maximum value used so far is m (0 ≤ x,y,m ≤ 7).
 *            Initial state: a[0]=0, a[1]∈{0,1}. The recurrence enforces (i)
 *            no three consecutive unequal elements, (ii) new valu...
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 39, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>

#define V 8                           /* number of possible values (0..7) */

seq_t a(idx_t n)
{
    static seq_t dp[V][V][V];
    static seq_t ndp[V][V][V];

    /* reset DP tables */
    memset(dp, 0, sizeof(dp));
    memset(ndp, 0, sizeof(ndp));

    /* initial two positions: a[0] = 0, a[1] is 0 or 1 */
    dp[0][0][0] = (seq_t)1;           /* 0,0  .. max 0 */
    dp[0][1][1] = (seq_t)1;           /* 0,1  .. max 1 */

    /* extend the array up to length n+2 (positions 0..n+1) */
    for (idx_t pos = 2; pos <= n + 1; ++pos) {
        memset(ndp, 0, sizeof(ndp));      /* clear next step */

        for (int x = 0; x < V; ++x) {
            for (int y = 0; y < V; ++y) {
                for (int m = 0; m < V; ++m) {
                    seq_t cur = dp[x][y][m];
                    if (!cur) continue;

                    int max_new = m + 1;
                    if (max_new > 7) max_new = 7;

                    for (int z = 0; z <= max_new; ++z) {
                        /* no triple of three distinct values */
                        if (x == y || x == z || y == z) {
                            int m2 = (z > m) ? z : m;
                            ndp[y][z][m2] += cur;
                        }
                    }
                }
            }
        }

        /* swap dp and ndp for next iteration */
        memcpy(dp, ndp, sizeof(dp));
    }

    /* sum all states after the last position */
    seq_t total = 0;
    for (int x = 0; x < V; ++x)
        for (int y = 0; y < V; ++y)
            for (int m = 0; m < V; ++m)
                total += dp[x][y][m];

    return total;
}
