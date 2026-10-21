/*
 * A253160  Number of (n+1) X (2+1) 0..2 arrays with every 2 X 2 subblock sum
 *          differing from its neighbors horizontally, vertically and nw-se
 *          diagonally.
 * https://oeis.org/A253160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×(2+1) 0..2 arrays with every 2×2 subblock
 *            sum differing from its horizontal, vertical and NW–SE diagonal
 *            neighbours. This implementation uses a DP over rows: each row is
 *            encoded as 3 base‑3 digits (27 possibilities). The state is the
 *            last two rows; transitions are allowed o...
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include <stdbool.h>


seq_t a(idx_t n)
{
    /* Pre‑compute subblock sums and allowed transitions once. */
    static bool initialized = false;
    static unsigned char sum0[27][27], sum1[27][27];
    static unsigned char next_cnt[27][27];
    static int next_vals[27][27][27];

    if (!initialized) {
        int digits[27][3];
        for (int r = 0; r < 27; ++r) {
            int t = r;
            digits[r][0] = t % 3; t /= 3;
            digits[r][1] = t % 3; t /= 3;
            digits[r][2] = t % 3;
        }
        for (int a = 0; a < 27; ++a) {
            for (int b = 0; b < 27; ++b) {
                sum0[a][b] = digits[a][0] + digits[a][1] +
                              digits[b][0] + digits[b][1];
                sum1[a][b] = digits[a][1] + digits[a][2] +
                              digits[b][1] + digits[b][2];
            }
        }
        for (int a = 0;  a < 27; ++a) {
            for (int b = 0;  b < 27; ++b) {
                int cnt = 0;
                unsigned char s0_back = sum0[a][b];
                unsigned char s1_back = sum1[a][b];
                for (int c = 0; c < 27; ++c) {
                    unsigned char s0_curr = sum0[b][c];
                    unsigned char s1_curr = sum1[b][c];
                    if (s0_curr == s1_curr)          continue; /* vertical, new pair  */
                    if (s0_back == s0_curr)          continue; /* horizontal       */
                    if (s1_back == s1_curr)          continue;
                    if (s0_back == s1_curr)          continue; /* diagonal         */
                    next_vals[a][b][cnt++] = c;
                }
                next_cnt[a][b] = cnt;
            }
        }
        initialized = true;
    }

    static seq_t dp[27][27];
    static seq_t next[27][27];

    /* Initialise DP with the first pair of rows. */
    for (int i = 0; i < 27; ++i)
        for (int j = 0; j < 27; ++j)
            dp[i][j] = 0;

    for (int a = 0; a < 27; ++a)
        for (int b = 0; b < 27; ++b)
            if (sum0[a][b] != sum1[a][b])        /* vertical rule   */
                dp[a][b] = 1;

    /* Add the remaining rows. */
    for (idx_t step = 2; step <= n; ++step) {   /* total rows = n+1 */
        for (int i = 0; i < 27; ++i)
            for (int j = 0; j < 27; ++j)
                next[i][j] = 0;

        for (int a = 0; a < 27; ++a) {
            for (int b = 0; b < 27; ++b) {
                seq_t cur = dp[a][b];
                if (cur == 0) continue;
                int cnt = next_cnt[a][b];
                for (int k = 0; k < cnt; ++k) {
                    int c = next_vals[a][b][k];
                    next[b][c] += cur;
                }
            }
        }

        /* move to next state */
        for (int i = 0; i < 27; ++i)
            for (int j = 0; j < 27; ++j)
                dp[i][j] = next[i][j];
    }

    /* Sum all valid final states. */
    seq_t total = 0;
    for (int i = 0; i < 27; ++i)
        for (int j = 0; j < 27; ++j)
            total += dp[i][j];

    return total;
}
