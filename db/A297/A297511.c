/*
 * A297511  Number of 6Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 1 or 2 neighboring 1s.
 * https://oeis.org/A297511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count all 6×n binary arrays where every 1 has 1 or 2 * neighbors
 *            among horizontally and diagonally adjacent cells. * The problem
 *            is solved by dynamic programming over column patterns. * * Each
 *            column is represented by a 6‑bit mask (rows 0..5). * For a fixed
 *            triple of consecutive columns (q,p,r) t...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

static inline int valid_transition(int q, int p, int r)
{
    for (int i = 0; i < 6; ++i) {
        if ((p >> i) & 1) {
            int cnt = 0;
            cnt += (q >> i) & 1;          /* left neighbor same row */
            cnt += (r >> i) & 1;          /* right neighbor same row */
            if (i > 0)  cnt += (q >> (i - 1)) & 1; /* up-left */
            if (i < 5)  cnt += (q >> (i + 1)) & 1; /* down-left */
            if (i > 0)  cnt += (r >> (i - 1)) & 1; /* up-right */
            if (i < 5)  cnt += (r >> (i + 1)) & 1; /* down-right */
            if (cnt < 1 || cnt > 2) return 0;
        }
    }
    return 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Pre‑compute transition masks:
     * mask[p][r] has bit q set iff transition(q,p,r) is valid.
     */
    static uint64_t mask[64][64];
    static int mask_inited = 0;
    if (!mask_inited) {
        memset(mask, 0, sizeof(mask));
        for (int q = 0; q < 64; ++q) {
            for (int p = 0; p < 64; ++p) {
                for (int r = 0; r < 64; ++r) {
                    if (valid_transition(q, p, r))
                        mask[p][r] |= (1ULL << q);
                }
            }
        }
        mask_inited = 1;
    }

    /* DP arrays: dp[q][p] stores counts for state (q,p).
     * newdp is the next state array.
     */
    static seq_t dp[64][64];
    static seq_t newdp[64][64];
    memset(dp, 0, sizeof(dp));

    /* Initial state: column 0 with prev_prev=0 */
    for (int p = 0; p < 64; ++p)
        dp[0][p] = (seq_t)1;

    /* Iterate over columns 1 .. n-1 (real columns). */
    for (idx_t col = 1; col < n; ++col) {
        memset(newdp, 0, sizeof(newdp));
        for (int p = 0; p < 64; ++p) {
            for (int r = 0; r < 64; ++r) {
                uint64_t m = mask[p][r];
                seq_t sum = 0;
                while (m) {
                    unsigned q = __builtin_ctzll(m);
                    sum += dp[q][p];
                    m &= m - 1;
                }
                newdp[p][r] = sum;
            }
        }
        /* Prepare for next iteration */
        memcpy(dp, newdp, sizeof(dp));
    }

    /* Final dummy column 0 to finalize last real column. */
    seq_t result = 0;
    for (int p = 0; p < 64; ++p) {
        uint64_t m = mask[p][0];
        while (m) {
            unsigned q = __builtin_ctzll(m);
            result += dp[q][p];
            m &= m - 1;
        }
    }

    return result;
}
