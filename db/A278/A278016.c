/*
 * A278016  Number of 3Xn 0..2 arrays with every element plus 1 mod 3 equal to
 *          some element at offset (-1,-1) (-1,0) (-1,1) (0,-1) (0,1) or
 *          (1,0), with upper left element zero.
 * https://oeis.org/A278016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   col_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A278016: dynamic programming over column types for
   3×n arrays with the adjacency rule
   (+1 mod 3 of each element must appear at one of the offsets
   (-1,-1), (-1,0), (-1,1), (0,-1), (0,1) or (1,0)),
   with the upper‑left element equal to 0. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/col_ok.h"

#define COLS 27    /* 3³ possible column types */

seq_t a(idx_t n)
{
    static bool ready = false;
    static int cols[COLS][3];
    static unsigned char single_ok[COLS];
    static unsigned char first_ok[COLS][COLS];
    static unsigned char last_ok[COLS][COLS];
    static unsigned char middle_ok[COLS][COLS][COLS];

    if (!ready) {
        /* enumerate all columns */
        for (int i = 0; i < COLS; ++i) {
            int x = i;
            for (int r = 2; r >= 0; --r) {
                cols[i][r] = x % 3;
                x /= 3;
            }
        }
        int dummy[3] = {0, 0, 0};

        /* single column validity */
        for (int i = 0; i < COLS; ++i)
            single_ok[i] = col_ok(cols[i], dummy, dummy, 0, 0);

        /* first and last column compatibility */
        for (int i = 0; i < COLS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                first_ok[i][j] = col_ok(cols[i], dummy, cols[j], 0, 1);
                last_ok[i][j]  = col_ok(cols[j], cols[i], dummy, 1, 0);
            }
        }

        /* middle column compatibility */
        for (int i = 0; i < COLS; ++i)
            for (int j = 0; j < COLS; ++j)
                for (int k = 0; k < COLS; ++k)
                    middle_ok[i][j][k] = col_ok(cols[j], cols[i], cols[k], 1, 1);

        ready = true;
    }

    if (n == 0) return (seq_t)0;   /* offset not used */

    /* n == 1: only one column, top cell must be 0 */
    if (n == 1) {
        seq_t sum = 0;
        for (int i = 0; i < COLS; ++i)
            if (cols[i][0] == 0 && single_ok[i])
                ++sum;
        return sum;
    }

    /* dynamic programming arrays */
    size_t states = (size_t)COLS * (size_t)COLS;
    seq_t *dp = (seq_t *)calloc(states, sizeof(seq_t));
    seq_t *newdp = (seq_t *)calloc(states, sizeof(seq_t));
    if (!dp || !newdp) {
        free(dp); free(newdp);
        return (seq_t)0;
    }

    /* initialize first two columns */
    for (int a = 0; a < COLS; ++a) {
        if (cols[a][0] != 0) continue;       /* upper-left must be 0 */
        for (int b = 0; b < COLS; ++b) {
            if (!first_ok[a][b]) continue;
            dp[(size_t)a * COLS + b] += (seq_t)1;
        }
    }

    if (n == 2) {
        seq_t ans = 0;
        for (int a = 0; a < COLS; ++a)
            for (int b = 0; b < COLS; ++b)
                if (dp[(size_t)a * COLS + b] && last_ok[a][b])
                    ans += dp[(size_t)a * COLS + b];
        free(dp); free(newdp);
        return ans;
    }

    /* iterate columns 3..n */
    for (idx_t step = 3; step <= n; ++step) {
        memset(newdp, 0, states * sizeof(seq_t));
        for (int a = 0; a < COLS; ++a) {
            for (int b = 0; b < COLS; ++b) {
                seq_t cnt = dp[(size_t)a * COLS + b];
                if (!cnt) continue;
                for (int c = 0; c < COLS; ++c) {
                    if (middle_ok[a][b][c]) {
                        newdp[(size_t)b * COLS + c] += cnt;
                    }
                }
            }
        }
        seq_t *tmp = dp; dp = newdp; newdp = tmp;
    }

    /* final sum over last column */
    seq_t ans = 0;
    for (int a = 0; a < COLS; ++a)
        for (int b = 0; b < COLS; ++b)
            if (dp[(size_t)a * COLS + b] && last_ok[a][b])
                ans += dp[(size_t)a * COLS + b];

    free(dp); free(newdp);
    return ans;
}
