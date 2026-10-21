/*
 * A284072  Number of n X 5 0..1 arrays with no 1 equal to more than three of
 *          its horizontal, diagonal and antidiagonal neighbors, with the
 *          exception of exactly two elements.
 * https://oeis.org/A284072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 0/1 arrays where each 1 has at most three *
 *            neighbours among horizontal, diagonal and antidiagonal
 *            positions, * except for exactly two such 1’s that may have more
 *            than three neighbours
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static int viol[32][32][32];
static bool vis_viol = false;

static void init_viol(void) {
    if (vis_viol) return;
    for (int p2 = 0; p2 < 32; ++p2) {
        for (int p = 0; p < 32; ++p) {
            for (int cur = 0; cur < 32; ++cur) {
                int cnt = 0;
                for (int c = 0; c < 5; ++c) {
                    if (p & (1 << c)) {
                        int nb = 0;
                        /* horizontal */
                        if (c > 0 && (p & (1 << (c - 1)))) nb++;
                        if (c < 4 && (p & (1 << (c + 1)))) nb++;
                        /* diagonal up */
                        if (c > 0 && (p2 & (1 << (c - 1)))) nb++;
                        if (c < 4 && (p2 & (1 << (c + 1)))) nb++;
                        /* diagonal down */
                        if (c > 0 && (cur & (1 << (c - 1)))) nb++;
                        if (c < 4 && (cur & (1 << (c + 1)))) nb++;
                        if (nb > 3) cnt++;
                    }
                }
                viol[p2][p][cur] = cnt;
            }
        }
    }
    vis_viol = true;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1 */
    init_viol();

    /* dp[2][32][32][3] : we alternate the first dimension for current/next */
    static seq_t dp[2][32][32][3];
    for (int d = 0; d < 2; ++d)
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                for (int k = 0; k < 3; ++k)
                    dp[d][i][j][k] = 0;

    int cur = 0, nxt = 1;
    /* initial state after the first row: prev2 = 0, prev = first row mask */
    for (int m = 0; m < 32; ++m) dp[cur][0][m][0] = 1;

    /* Process rows 2 .. n */
    for (idx_t row = 2; row <= n; ++row) {
        /* clear next dp */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                for (int k = 0; k < 3; ++k)
                    dp[nxt][i][j][k] = 0;

        for (int p2 = 0; p2 < 32; ++p2) {
            for (int p = 0; p < 32; ++p) {
                for (int v = 0; v < 3; ++v) {
                    seq_t cnt = dp[cur][p2][p][v];
                    if (!cnt) continue;
                    for (int c = 0; c < 32; ++c) {
                        int add = viol[p2][p][c];
                        int nv = v + add;
                        if (nv <= 2)
                            dp[nxt][p][c][nv] += cnt;
                    }
                }
            }
        }
        int tmp = cur; cur = nxt; nxt = tmp;
    }

    /* Final evaluation with bottom virtual zero row */
    seq_t ans = 0;
    for (int p2 = 0; p2 < 32; ++p2) {
        for (int p = 0; p < 32; ++p) {
            for (int v = 0; v < 3; ++v) {
                seq_t cnt = dp[cur][p2][p][v];
                if (!cnt) continue;
                int add = viol[p2][p][0];
                int nv = v + add;
                if (nv == 2)
                    ans += cnt;
            }
        }
    }
    return ans;
}
