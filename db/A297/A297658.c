/*
 * A297658  Number of 5Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 1, 2 or 3 neighboring 1s.
 * https://oeis.org/A297658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: transfer‑matrix DP over 32 column masks. */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

static bool _initialized = false;
static unsigned char valid[32][32][32];

/* Return true if for every 1 in middle mask the number of 1‑neighbors
   among the left and right masks lies between 1 and 3. */
static inline bool check_valid(int left, int mid, int right)
{
    for (int r = 0; r < 5; ++r) {
        if ((mid >> r) & 1) {
            int cnt = 0;
            /* horizontal neighbors */
            cnt += (left >> r) & 1;
            cnt += (right >> r) & 1;
            /* diagonal neighbors */
            if (r > 0) {
                cnt += (left >> (r - 1)) & 1;
                cnt += (right >> (r - 1)) & 1;
            }
            if (r < 4) {
                cnt += (left >> (r + 1)) & 1;
                cnt += (right >> (r + 1)) & 1;
            }
            if (cnt < 1 || cnt > 3)
                return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Initialize the compatibility table once */
    if (!_initialized) {
        for (int left = 0; left < 32; ++left)
            for (int mid = 0; mid < 32; ++mid)
                for (int right = 0; right < 32; ++right)
                    valid[left][mid][right] = check_valid(left, mid, right);
        _initialized = true;
    }

    seq_t dp[32][32];
    seq_t newdp[32][32];

    /* zero out dp */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = (seq_t)0;

    /* first column has no left neighbor */
    int left0 = 0;
    for (int m1 = 0; m1 < 32; ++m1)
        for (int m2 = 0; m2 < 32; ++m2)
            if (valid[left0][m1][m2])
                dp[m1][m2] = (seq_t)1;

    /* Process columns 3 .. n */
    for (idx_t col = 3; col <= n; ++col) {
        /* reset newdp */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                newdp[i][j] = (seq_t)0;

        for (int prev = 0; prev < 32; ++prev) {
            for (int cur = 0; cur < 32; ++cur) {
                seq_t ways = dp[prev][cur];
                if (ways == (seq_t)0) continue;
                for (int nxt = 0; nxt < 32; ++nxt) {
                    if (valid[prev][cur][nxt]) {
                        newdp[cur][nxt] += ways;
                    }
                }
            }
        }

        /* copy newdp into dp */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* sum over last column states (right neighbor is 0) */
    seq_t ans = (seq_t)0;
    for (int prev = 0; prev < 32; ++prev) {
        for (int cur = 0; cur < 32; ++cur) {
            seq_t ways = dp[prev][cur];
            if (ways == (seq_t)0) continue;
            if (valid[prev][cur][0])
                ans += ways;
        }
    }

    return ans;
}
