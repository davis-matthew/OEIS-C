/*
 * A297724  Number of 5Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 1, 2 or 4 neighboring 1s.
 * https://oeis.org/A297724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts 5×n binary matrices in which every 1 has exactly *
 *            1, 2, or 4 neighboring 1s (neighbors are horizontally,
 *            diagonally, * or antidiagonally adjacent cells). This code
 *            implements that count * by dynamic programming over column
 *            patterns
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Masks for 5-bit rows: 0..31 */
    static uint32_t validMask[32][32] = {0};
    static bool initialized = false;
    if (!initialized) {
        /* Precompute for each pair (a,b) the mask of c such that
           the triple (a,b,c) is valid. */
        for (unsigned a = 0; a < 32; ++a) {
            for (unsigned b = 0; b < 32; ++b) {
                uint32_t mask = 0;
                for (unsigned c = 0; c < 32; ++c) {
                    bool ok = true;
                    for (int r = 0; r < 5 && ok; ++r) {
                        if ((b >> r) & 1U) {
                            int cnt = 0;
                            /* left column a */
                            if ((a >> r) & 1U) ++cnt;
                            if (r > 0 && ((a >> (r - 1)) & 1U)) ++cnt;
                            if (r < 4 && ((a >> (r + 1)) & 1U)) ++cnt;
                            /* right column c */
                            if ((c >> r) & 1U) ++cnt;
                            if (r > 0 && ((c >> (r - 1)) & 1U)) ++cnt;
                            if (r < 4 && ((c >> (r + 1)) & 1U)) ++cnt;
                            if (!(cnt == 1 || cnt == 2 || cnt == 4))
                                ok = false;
                        }
                    }
                    if (ok)
                        mask |= (1U << c);
                }
                validMask[a][b] = mask;
            }
        }
        initialized = true;
    }

    /* dp[prev][curr] holds counts for the last two columns processed. */
    static seq_t dp[32][32];
    static seq_t newdp[32][32];
    for (unsigned i = 0; i < 32; ++i)
        for (unsigned j = 0; j < 32; ++j)
            dp[i][j] = 0;

    /* First column: previous previous column is dummy 0. */
    for (unsigned b = 0; b < 32; ++b)
        dp[0][b] = 1;

    /* Process columns 2..n. */
    for (idx_t col = 2; col <= n; ++col) {
        for (unsigned i = 0; i < 32; ++i)
            for (unsigned j = 0; j < 32; ++j)
                newdp[i][j] = 0;

        for (unsigned a = 0; a < 32; ++a) {
            for (unsigned b = 0; b < 32; ++b) {
                seq_t cur = dp[a][b];
                if (cur == 0) continue;
                uint32_t mask = validMask[a][b];
                for (unsigned c = 0; c < 32; ++c) {
                    if (mask & (1U << c))
                        newdp[b][c] += cur;
                }
            }
        }

        /* Copy newdp to dp for next iteration. */
        for (unsigned i = 0; i < 32; ++i)
            for (unsigned j = 0; j < 32; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* Sum over final column: use dummy 0 as the column after the last. */
    seq_t ans = 0;
    for (unsigned a = 0; a < 32; ++a) {
        for (unsigned b = 0; b < 32; ++b) {
            seq_t cur = dp[a][b];
            if (cur == 0) continue;
            if (validMask[a][b] & 1U)   /* mask bit for c = 0 */
                ans += cur;
        }
    }
    return ans;
}
