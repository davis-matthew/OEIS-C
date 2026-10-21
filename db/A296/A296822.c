/*
 * A296822  Number of nX3 0..1 arrays with each 1 adjacent to 2, 3 or 4
 *          king-move neighboring 1s.
 * https://oeis.org/A296822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts n×3 binary arrays where each 1 has 2–4 king‑move
 *            neighboring 1’s. It is computed by a linear DP over states of
 *            the last two rows. No hard‑coded terms
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Count of n × 3 binary arrays with the adjacency rule.  */
seq_t a(idx_t n)
{
    /* Pre‑compute validity of every triple of rows.  */
    static bool valid_triple[8][8][8];
    static bool valid_init = false;
    if (!valid_init) {
        for (int prev = 0; prev < 8; ++prev)
            for (int curr = 0; curr < 8; ++curr)
                for (int next = 0; next < 8; ++next) {
                    bool ok = true;
                    for (int j = 0; j < 3 && ok; ++j)
                        if (curr & (1 << j)) {
                            int cnt = 0;
                            for (int dr = -1; dr <= 1; ++dr) {
                                int r0 = dr + 1;   /* 0:prev, 1:curr, 2:next */
                                for (int dc = -1; dc <= 1; ++dc) {
                                    if (dr == 0 && dc == 0) continue;
                                    int cj = j + dc;
                                    if (cj < 0 || cj > 2) continue;
                                    if (dr == -1) {
                                        if ((prev >> cj) & 1u) ++cnt;
                                    } else if (dr == 0) {
                                        if ((curr >> cj) & 1u) ++cnt;
                                    } else {
                                        if ((next >> cj) & 1u) ++cnt;
                                    }
                                }
                            }
                            if (cnt < 2 || cnt > 4) ok = false;
                        }
                    valid_triple[prev][curr][next] = ok;
                }
        valid_init = true;
    }

    /* DP arrays: dp[prev][curr] holds the number of ways to reach the
       current state, where prev is the row i‑2 and curr is the row i‑1.  */
    static seq_t dp[8][8];
    static seq_t ndp[8][8];
    /* initialise dp to zero */
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            dp[i][j] = 0;
    dp[0][0] = 1;   /* sentinel rows before the first real row */

    /* Add the real rows.  */
    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                ndp[i][j] = 0;
        for (int prev = 0; prev < 8; ++prev)
            for (int curr = 0; curr < 8; ++curr) {
                seq_t v = dp[prev][curr];
                if (!v) continue;
                for (int next = 0; next < 8; ++next)
                    if (valid_triple[prev][curr][next])
                        ndp[curr][next] = ndp[curr][next] + v;
            }
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Add the final sentinel row (0) to validate the last real row.  */
    seq_t ans = 0;
    for (int prev = 0; prev < 8; ++prev)
        for (int curr = 0; curr < 8; ++curr) {
            seq_t v = dp[prev][curr];
            if (!v) continue;
            if (valid_triple[prev][curr][0])  /* next = 0 sentinel */
                ans = ans + v;
        }
    return ans;
}
