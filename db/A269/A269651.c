/*
 * A269651  Number of length-n 0..3 arrays with no adjacent pair x,x+1
 *          repeated.
 * https://oeis.org/A269651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 34, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dynamic programming: count length‑n arrays over {0,1,2,3}
   where each of the adjacent pairs 01, 12, 23 occurs at most once.
   DP state: dp[mask][last] for mask (bits 0..2 indicate whether
   pair 01, 12, or 23 has already appeared) and last element (0..3). */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; behaviour below 1 is irrelevant */
        return (seq_t)0;

    /* dp[mask][last] has at most 8*4 = 32 entries */
    seq_t dp[8][4];
    for (int m = 0; m < 8; ++m)
        for (int c = 0; c < 4; ++c)
            dp[m][c] = (seq_t)0;

    /* length = 1: any single element, no pairs used yet */
    for (int c = 0; c < 4; ++c)
        dp[0][c] = (seq_t)1;

    /* extend sequences up to length n */
    for (idx_t len = 1; len < n; ++len) {
        seq_t next[8][4];
        for (int m = 0; m < 8; ++m)
            for (int c = 0; c < 4; ++c)
                next[m][c] = (seq_t)0;

        for (int m = 0; m < 8; ++m) {
            for (int last = 0; last < 4; ++last) {
                seq_t val = dp[m][last];
                if (val == (seq_t)0) continue;

                for (int x = 0; x < 4; ++x) {
                    int m2 = m;
                    if (last + 1 == x) {            /* increasing pair */
                        int bit;                     /* which pair? */
                        if (last == 0) bit = 0;
                        else if (last == 1) bit = 1;
                        else if (last == 2) bit = 2;
                        else bit = -1;               /* last==3 -> no pair */

                        if (bit >= 0) {
                            if (m & (1 << bit)) continue; /* pair already used */
                            m2 |= (1 << bit);
                        }
                    }
                    next[m2][x] += val;
                }
            }
        }
        /* copy back to dp */
        for (int m = 0; m < 8; ++m)
            for (int c = 0; c < 4; ++c)
                dp[m][c] = next[m][c];
    }

    /* Sum all end states */
    seq_t res = (seq_t)0;
    for (int m = 0; m < 8; ++m)
        for (int c = 0; c < 4; ++c)
            res += dp[m][c];

    return res;
}
