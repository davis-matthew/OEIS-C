/*
 * A283695  Number of 5Xn 0..1 arrays with no 1 equal to more than two of its
 *          horizontal, diagonal and antidiagonal neighbors.
 * https://oeis.org/A283695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 5×n 0/1 arrays with no 1 having more than two
 *            of its horizontal, diagonal, or antidiagonal neighbors (vertical
 *            neighbors are not counted). We compute a(n) by dynamic
 *            programming over column patterns. The recurrence is: a(1) = 32,
 *            a(n+1) = sum_{prev,curr,next} a(n)_{prev,curr} if...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* PUBLIC API required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    /* Precompute validity of each triple (curr,prev,next).  */
    static bool ok[32][32][32];
    static bool ok_computed = false;
    if (!ok_computed) {
        for (int curr = 0; curr < 32; ++curr) {
            for (int prev = 0; prev < 32; ++prev) {
                for (int next = 0; next < 32; ++next) {
                    bool good = true;
                    for (int r = 0; r < 5 && good; ++r) {
                        if ((curr >> r) & 1) {          /* cell is 1 */
                            int cnt = 0;
                            /* horizontal neighbors */
                            if ((prev >> r) & 1) ++cnt;      /* left */
                            if ((next >> r) & 1) ++cnt;      /* right */
                            /* diagonal and antidiagonal neighbors */
                            if (r > 0) {
                                if ((prev >> (r - 1)) & 1) ++cnt;   /* up-left */
                                if ((next >> (r - 1)) & 1) ++cnt;   /* up-right */
                            }
                            if (r < 4) {
                                if ((prev >> (r + 1)) & 1) ++cnt;   /* down-left */
                                if ((next >> (r + 1)) & 1) ++cnt;   /* down-right */
                            }
                            if (cnt > 2) good = false;
                        }
                    }
                    ok[curr][prev][next] = good;
                }
            }
        }
        ok_computed = true;
    }

    /* DP tables: dp[prev][curr] is number of ways after processing
       the last column with patterns 'prev' (column i-1) and 'curr'
       (column i).  */
    seq_t dp[32][32], ndp[32][32];
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = 0;
    dp[0][0] = (seq_t)1;  /* virtual column before the first real one */

    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                ndp[i][j] = 0;

        for (int prev = 0; prev < 32; ++prev) {
            for (int curr = 0; curr < 32; ++curr) {
                seq_t ways = dp[prev][curr];
                if (ways == 0) continue;
                for (int next = 0; next < 32; ++next) {
                    if (ok[curr][prev][next]) {
                        ndp[curr][next] += ways;
                    }
                }
            }
        }
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Final validation: the last column (pattern 'curr')
       must be valid with left neighbor 'prev' and right neighbor 0. */
    seq_t ans = 0;
    for (int prev = 0; prev < 32; ++prev) {
        for (int curr = 0; curr < 32; ++curr) {
            seq_t ways = dp[prev][curr];
            if (ways == 0) continue;
            if (ok[curr][prev][0]) ans += ways;
        }
    }
    return ans;
}
