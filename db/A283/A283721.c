/*
 * A283721  Number of n X 3 0..1 arrays with no 1 equal to more than two of
 *          its horizontal, diagonal and antidiagonal neighbors, with the
 *          exception of exactly one element.
 * https://oeis.org/A283721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 binary arrays with exactly one 1 whose *
 *            number of horizontal, diagonal, and antidiagonal neighbors is
 *            more * than two. The array is counted when exactly one such
 *            violation * occurs. The computation is performed by a row‑wise
 *            dynamic program * that keeps track of the two most...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint8_t viol[8][8][8];
static bool viol_init = false;

static void init_viol(void)
{
    if (viol_init) return;
    for (int a = 0; a < 8; ++a) {
        for (int b = 0; b < 8; ++b) {
            for (int c = 0; c < 8; ++c) {
                uint8_t add = 0;
                for (int col = 0; col < 3; ++col) {
                    if ((b >> col) & 1U) {
                        int cnt = 0;
                        /* horizontal */
                        if (col > 0 && ((b >> (col - 1)) & 1U)) cnt++;
                        if (col < 2 && ((b >> (col + 1)) & 1U)) cnt++;
                        /* diagonal above */
                        if (col > 0 && ((a >> (col - 1)) & 1U)) cnt++;
                        if (col < 2 && ((a >> (col + 1)) & 1U)) cnt++;
                        /* diagonal below */
                        if (col > 0 && ((c >> (col - 1)) & 1U)) cnt++;
                        if (col < 2 && ((c >> (col + 1)) & 1U)) cnt++;
                        if (cnt > 2) add++;
                    }
                }
                viol[a][b][c] = add;
            }
        }
    }
    viol_init = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    init_viol();

    /* dp[prev_prev][prev][v]  v = 0 or 1 indicating violations seen so far */
    static seq_t dp[8][8][2];
    static seq_t ndp[8][8][2];

    /* zero arrays */
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            for (int k = 0; k < 2; ++k)
                dp[i][j][k] = 0;

    /* first row */
    for (int p = 0; p < 8; ++p)
        dp[0][p][0] = 1;

    /* process rows 2 .. n */
    for (idx_t row = 2; row <= n; ++row) {
        /* zero ndp */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                for (int k = 0; k < 2; ++k)
                    ndp[i][j][k] = 0;

        for (int pp = 0; pp < 8; ++pp) {
            for (int p = 0; p < 8; ++p) {
                for (int v = 0; v < 2; ++v) {
                    seq_t ways = dp[pp][p][v];
                    if (!ways) continue;
                    for (int cur = 0; cur < 8; ++cur) {
                        uint8_t add = viol[pp][p][cur];
                        if (v + add > 1) continue;
                        ndp[p][cur][v + add] += ways;
                    }
                }
            }
        }
        /* copy ndp to dp for next iteration */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                for (int k = 0; k < 2; ++k)
                    dp[i][j][k] = ndp[i][j][k];
    }

    /* final evaluation of the last row against a sentinel row of zeros */
    seq_t ans = 0;
    for (int pp = 0; pp < 8; ++pp) {
        for (int p = 0; p < 8; ++p) {
            for (int v = 0; v < 2; ++v) {
                seq_t ways = dp[pp][p][v];
                if (!ways) continue;
                uint8_t add = viol[pp][p][0];
                if (v + add == 1)  /* exactly one violation in the entire array */
                    ans += ways;
            }
        }
    }
    return ans;
}
