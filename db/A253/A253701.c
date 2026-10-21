/*
 * A253701  Number of (4+1)X(n+1) 0..1 arrays with every 2X2 subblock ne-sw
 *          antidiagonal difference unequal to its neighbors horizontally and
 *          nw+se diagonal sum unequal to its neighbors vertically.
 * https://oeis.org/A253701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of 5×(n+1) binary arrays such that for every
 *            2×2 subblock the NE–SW antidiagonal difference is different from
 *            the difference of the horizontally adjacent subblock, and the
 *            NW+SE diagonal sum is different from the sum of the vertically
 *            adjacent subblock. This is computed by a dy...
 * Range:     u8 none, u16 n <= 16, u32 n <= 45, u64 n <= 104, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Pre‑compute data once. */
    static bool initialized = false;
    static bool vertical_ok[32][32];
    static int8_t diff_pair[32][32][4];
    static uint32_t allowed_mask[32][32];

    if (!initialized) {
        for (int p = 0; p < 32; ++p) {
            for (int q = 0; q < 32; ++q) {
                bool ok = true;
                int prev_sum = -1;          /* no previous sum for i==0 */
                for (int i = 0; i < 4; ++i) {
                    int sum  = ((p >> i) & 1) + ((q >> (i + 1)) & 1);
                    diff_pair[p][q][i] = (int8_t)(((q >> i) & 1) - ((p >> (i + 1)) & 1));
                    if (i > 0 && sum == prev_sum) ok = false;
                    prev_sum = sum;
                }
                vertical_ok[p][q] = ok;
            }
        }

        for (int a = 0; a < 32; ++a) {
            for (int b = 0; b < 32; ++b) {
                uint32_t mask = 0;
                if (!vertical_ok[a][b]) {
                    allowed_mask[a][b] = 0;
                    continue;
                }
                for (int c = 0; c < 32; ++c) {
                    if (!vertical_ok[b][c]) continue;
                    bool diff_ok = true;
                    for (int i = 0; i < 4; ++i) {
                        if (diff_pair[b][c][i] == diff_pair[a][b][i]) {
                            diff_ok = false;
                            break;
                        }
                    }
                    if (diff_ok) mask |= (1u << c);
                }
                allowed_mask[a][b] = mask;
            }
        }
        initialized = true;
    }

    /* Dynamic programming over column pairs. */
    static seq_t dp[32][32];
    static seq_t ndp[32][32];

    /* Initial state: first pair of columns (column 0 and 1). */
    for (int a = 0; a < 32; ++a)
        for (int b = 0; b < 32; ++b)
            dp[a][b] = vertical_ok[a][b] ? (seq_t)1 : (seq_t)0;

    if (n == 1) {
        seq_t ans = 0;
        for (int a = 0; a < 32; ++a)
            for (int b = 0; b < 32; ++b)
                ans += dp[a][b];
        return ans;
    }

    for (idx_t step = 2; step <= n; ++step) {
        /* Clear next DP layer. */
        for (int a = 0; a < 32; ++a)
            for (int b = 0; b < 32; ++b)
                ndp[a][b] = (seq_t)0;

        for (int a = 0; a < 32; ++a) {
            for (int b = 0; b < 32; ++b) {
                seq_t cur = dp[a][b];
                if (cur == (seq_t)0) continue;
                uint32_t mask = allowed_mask[a][b];
                if (mask == 0) continue;
                for (int c = 0; c < 32; ++c) {
                    if (mask & (1u << c))
                        ndp[b][c] += cur;
                }
            }
        }

        /* Swap dp and ndp. */
        for (int a = 0; a < 32; ++a)
            for (int b = 0; b < 32; ++b)
                dp[a][b] = ndp[a][b];
    }

    /* Sum all end states. */
    seq_t ans = 0;
    for (int a = 0; a < 32; ++a)
        for (int b = 0; b < 32; ++b)
            ans += dp[a][b];
    return ans;
}
