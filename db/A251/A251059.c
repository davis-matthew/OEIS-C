/*
 * A251059  Number of (n+1)X(2+1) 0..2 arrays with no 2X2 subblock having
 *          x11-x00 less than x10-x01.
 * https://oeis.org/A251059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1) x 3 arrays of digits 0..2 such that in
 *            every 2×2 subblock the inequality (bottom‑right + top‑right) ≥
 *            (top‑left + bottom‑left) holds. Computed by dynamic programming
 *            over rows
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>   /* for uint8_t, etc. */

seq_t a(idx_t n)
{
    /* Precompute the directed adjacency matrix between rows.
       Rows are encoded as numbers 0..26 in base‑3:
       row r has digits a[0],a[1],a[2] where
           a[0] = r % 3, a[1] = (r/3) % 3, a[2] = r/9.   */
    static bool ready = false;
    static bool adj[27][27];
    if (!ready) {
        for (int s = 0; s < 27; ++s) {
            int sa[3];
            sa[0] = s % 3;
            sa[1] = (s / 3) % 3;
            sa[2] = s / 9;
            for (int t = 0; t < 27; ++t) {
                int tb[3];
                tb[0] = t % 3;
                tb[1] = (t / 3) % 3;
                tb[2] = t / 9;
                /* Check the two 2x2 subblocks that involve rows s and t. */
                bool ok = true;
                for (int j = 0; j < 2 && ok; ++j) {
                    /* Condition: bottom‑right + top‑right ≥ top‑left + bottom‑left */
                    if (tb[j+1] + sa[j+1] < sa[j] + tb[j]) ok = false;
                }
                adj[s][t] = ok;
            }
        }
        ready = true;
    }

    /* Dynamic programming over rows.  dp[r] = number of arrays
       that end with row r after the current number of processed rows. */
    seq_t dp[27];
    seq_t ndp[27];
    for (int r = 0; r < 27; ++r) dp[r] = 1;   /* first row can be anything */

    for (idx_t step = 1; step <= n; ++step) {
        for (int r = 0; r < 27; ++r) ndp[r] = 0;
        for (int s = 0; s < 27; ++s) {
            if (dp[s] == 0) continue;
            for (int t = 0; t < 27; ++t) {
                if (adj[s][t]) ndp[t] += dp[s];
            }
        }
        for (int r = 0; r < 27; ++r) dp[r] = ndp[r];
    }

    seq_t total = 0;
    for (int r = 0; r < 27; ++r) total += dp[r];
    return total;
}
