/*
 * A283095  Number of nX3 0..1 arrays with no 1 equal to more than one of its
 *          horizontal and vertical neighbors, with the exception of exactly
 *          two elements.
 * https://oeis.org/A283095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of n×3 binary arrays with no 1 equal to more than
 * one of its horizontal and vertical neighbors, with the exception of exactly
 * two elements.
 */
#include <stdbool.h>

static uint8_t horiz[8][3];
static bool horiz_init = false;

static void init_horiz(void)
{
    if (horiz_init) return;
    for (int mask = 0; mask < 8; ++mask) {
        for (int j = 0; j < 3; ++j) {
            if (!((mask >> j) & 1)) {
                horiz[mask][j] = 0;
            } else {
                int h = 0;
                if (j > 0 && ((mask >> (j - 1)) & 1)) h++;
                if (j < 2 && ((mask >> (j + 1)) & 1)) h++;
                horiz[mask][j] = (uint8_t)h;
            }
        }
    }
    horiz_init = true;
}

seq_t a(idx_t n)
{
    init_horiz();

    /* small DP tables: dp[before][prev][violations] */
    static seq_t dp[8][8][3];
    static seq_t ndp[8][8][3];

    /* clear dp */
    for (int b = 0; b < 8; ++b)
        for (int a = 0; a < 8; ++a)
            for (int v = 0; v < 3; ++v)
                dp[b][a][v] = 0;
    dp[0][0][0] = 1;

    /* process rows 1 .. n */
    for (idx_t row = 1; row <= n; ++row) {
        /* clear next table */
        for (int b = 0; b < 8; ++b)
            for (int a = 0; a < 8; ++a)
                for (int v = 0; v < 3; ++v)
                    ndp[b][a][v] = 0;

        for (int b = 0; b < 8; ++b) {
            for (int a = 0; a < 8; ++a) {
                for (int v = 0; v < 3; ++v) {
                    seq_t cur = dp[b][a][v];
                    if (cur == (seq_t)0) continue;

                    /* try every possible new row pattern c */
                    for (int c = 0; c < 8; ++c) {
                        int add = 0;  /* violations in row a if we add c */

                        /* count violations of row a (previous row) */
                        for (int j = 0; j < 3; ++j) {
                            if (!((a >> j) & 1)) continue;          /* only 1's matter */
                            int cnt = horiz[a][j];
                            if ((b >> j) & 1) cnt++;
                            if ((c >> j) & 1) cnt++;
                            if (cnt > 1) add++;
                        }

                        int nv = v + add;
                        if (nv > 2) continue;       /* too many violations */
                        ndp[a][c][nv] += cur;
                    }
                }
            }
        }

        /* copy ndp back to dp */
        for (int b = 0; b < 8; ++b)
            for (int a = 0; a < 8; ++a)
                for (int v = 0; v < 3; ++v)
                    dp[b][a][v] = ndp[b][a][v];
    }

    /* Finalize last row (prev state's prev_row) with below=0 */
    seq_t ans = 0;
    for (int b = 0; b < 8; ++b) {          /* row n-1 */
        for (int a = 0; a < 8; ++a) {      /* row n   */
            for (int v = 0; v < 3; ++v) {
                seq_t cur = dp[b][a][v];
                if (cur == (seq_t)0) continue;

                int add = 0;  /* violations in the last row a */
                for (int j = 0; j < 3; ++j) {
                    if (!((a >> j) & 1)) continue;
                    int cnt = horiz[a][j];
                    if ((b >> j) & 1) cnt++;   /* above neighbor */
                    /* below neighbor is 0 */
                    if (cnt > 1) add++;
                }

                int nv = v + add;
                if (nv == 2)
                    ans += cur;
            }
        }
    }

    return ans;
}
