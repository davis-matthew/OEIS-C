/*
 * A297764  Number of 3 X n 0..1 arrays with every 1 horizontally, diagonally
 *          or antidiagonally adjacent to 2, 3 or 4 neighboring 1's.
 * https://oeis.org/A297764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 3×n binary arrays in which every 1 has between
 *            2 and 4 horizontally, diagonally or anti-diagonally adjacent
 *            1‑s. Computed by a transfer‑matrix DP over column configurations
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   valid_col
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/valid_col.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* out of offset range – not used */
    if (n == 1) return 1;  /* only the all‑zero board */

    seq_t dp[8][8];
    seq_t ndp[8][8];

    /* initialise dp for first two columns */
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            dp[i][j] = 0;

    for (int col1 = 0; col1 < 8; ++col1)
        for (int col2 = 0; col2 < 8; ++col2)
            if (valid_col(col1, 0, col2))
                dp[col1][col2] = 1;

    /* add columns 3 .. n */
    for (idx_t pos = 3; pos <= n; ++pos) {
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                ndp[i][j] = 0;

        for (int prev = 0; prev < 8; ++prev) {
            for (int cur = 0; cur < 8; ++cur) {
                if (!dp[prev][cur]) continue;
                for (int nxt = 0; nxt < 8; ++nxt) {
                    if (valid_col(cur, prev, nxt))
                        ndp[cur][nxt] += dp[prev][cur];
                }
            }
        }

        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* finish: check last column with right neighbour = 0 */
    seq_t result = 0;
    for (int prev = 0; prev < 8; ++prev)
        for (int last = 0; last < 8; ++last)
            if (dp[prev][last] && valid_col(last, prev, 0))
                result += dp[prev][last];

    return result;
}
