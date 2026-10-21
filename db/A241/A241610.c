/*
 * A241610  Number of length n+2 0..4 arrays with no consecutive three
 *          elements summing to more than 4.
 * https://oeis.org/A241610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count length n+2 arrays over {0,…,4} with no three consecutive
 *            elements summing to more than 4. This is computed by dynamic
 *            programming on the last two elements
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 18, u64 n <= 39, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* DP state: dp[a][b] = number of length-2+step arrays ending with
       the two most recent values a,b.  Initially all 25 states are 1. */
    seq_t dp[5][5];
    seq_t ndp[5][5];
    for (idx_t i = 0; i < 5; ++i)
        for (idx_t j = 0; j < 5; ++j) {
            dp[i][j] = 1;   /* sequences of length 2: any pair is allowed */
            ndp[i][j] = 0;
        }

    /* Perform n extensions to reach length n+2.  Each step adds one more
       element c to the end of the current two‑element suffix (a,b).  The
       constraint a+b+c <= 4 must hold. */
    for (idx_t step = 0; step < n; ++step) {
        for (idx_t i = 0; i < 5; ++i)
            for (idx_t j = 0; j < 5; ++j)
                ndp[i][j] = 0;
        for (idx_t a = 0; a < 5; ++a)
            for (idx_t b = 0; b < 5; ++b) {
                seq_t ways = dp[a][b];
                if (ways == 0) continue;
                for (idx_t c = 0; c < 5; ++c)
                    if (a + b + c <= 4)
                        ndp[b][c] += ways;
            }
        /* Copy new counts back to dp for the next iteration. */
        for (idx_t i = 0; i < 5; ++i)
            for (idx_t j = 0; j < 5; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Sum all ending states to obtain the number of full-length arrays. */
    seq_t total = 0;
    for (idx_t i = 0; i < 5; ++i)
        for (idx_t j = 0; j < 5; ++j)
            total += dp[i][j];

    return total;
}
