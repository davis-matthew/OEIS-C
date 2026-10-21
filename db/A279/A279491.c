/*
 * A279491  Number of nX5 0..1 arrays with no element equal to a strict
 *          majority of its horizontal, vertical and antidiagonal neighbors,
 *          with the exception of exactly two elements, and with new values
 *          introduced in order 0 sequentially upwards.
 * https://oeis.org/A279491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   count of n×5 binary arrays with exactly two elements that are a
 *            strict majority of their horizontal, vertical and antidiagonal
 *            neighbours. The algorithm uses dynamic programming over rows,
 *            and the double count inherent to the symmetry of the
 *            construction is compensated by dividing the final resul...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   calc_viol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/calc_viol.h"

static unsigned char rowViolTop[32][32];
static unsigned char rowViolMid[32][32][32];
static unsigned char rowViolBottom[32][32];
static unsigned char rowViolSingle[32];
static bool tables_inited = false;

static void init_tables(void)
{
    if (tables_inited) return;
    tables_inited = true;

    for (int p = 0; p < 32; ++p)
        rowViolSingle[p] = (unsigned char)calc_viol((uint8_t)p,0,0,false,false);

    for (int t = 0; t < 32; ++t)
        for (int c = 0; c < 32; ++c) {
            rowViolTop[t][c]     = (unsigned char)calc_viol((uint8_t)t,0,(uint8_t)c,false,true);
            rowViolBottom[t][c]  = (unsigned char)calc_viol((uint8_t)c,(uint8_t)t,0,true,false);
            for (int n = 0; n < 32; ++n)
                rowViolMid[t][c][n] = (unsigned char)calc_viol((uint8_t)c,(uint8_t)t,(uint8_t)n,true,true);
        }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_tables();

    if (n == 0) return 0;

    if (n == 1) {
        seq_t cnt = 0;
        for (int p = 0; p < 32; ++p)
            if (rowViolSingle[p] == 2)
                ++cnt;
        return cnt / 2;   /* compensate double counting of exact‑2 patterns   */
    }

    static seq_t dp[32][32][3];
    static seq_t ndp[32][32][3];

    /* zero dp and ndp */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            for (int k = 0; k < 3; ++k) {
                dp[i][j][k] = 0;
                ndp[i][j][k] = 0;
            }

    /* initialise with first two rows */
    for (int first = 0; first < 32; ++first)
        for (int second = 0; second < 32; ++second) {
            int vio = rowViolTop[first][second];
            if (vio <= 2)
                dp[first][second][vio] += 1;
        }

    /* add remaining rows 3 .. n */
    for (idx_t rowIdx = 3; rowIdx <= n; ++rowIdx) {
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                for (int k = 0; k < 3; ++k)
                    ndp[i][j][k] = 0;

        for (int prev = 0; prev < 32; ++prev)
            for (int curr = 0; curr < 32; ++curr)
                for (int cnt = 0; cnt < 3; ++cnt) {
                    seq_t val = dp[prev][curr][cnt];
                    if (!val) continue;
                    for (int next = 0; next < 32; ++next) {
                        int vioCurr = rowViolMid[prev][curr][next];
                        int newCnt = cnt + vioCurr;
                        if (newCnt <= 2)
                            ndp[curr][next][newCnt] += val;
                    }
                }

        /* swap dp and ndp */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                for (int k = 0; k < 3; ++k)
                    dp[i][j][k] = ndp[i][j][k];
    }

    /* final evaluation for last row (no below neighbour) */
    seq_t ans = 0;
    for (int prev = 0; prev < 32; ++prev)
        for (int curr = 0; curr < 32; ++curr)
            for (int cnt = 0; cnt < 3; ++cnt) {
                seq_t val = dp[prev][curr][cnt];
                if (!val) continue;
                int vioLast = rowViolBottom[prev][curr];
                int total = cnt + vioLast;
                if (total == 2)
                    ans += val;
            }

    return ans / 2;   /* compensate double counting inherent to the DP   */
}
