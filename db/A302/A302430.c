/*
 * A302430  Number of 5Xn 0..1 arrays with every element equal to 0, 1, 3 or 5
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 5×n 0/1 arrays where each cell has 0, 1, 3 or 5
 *            equal‑valued horizontally, diagonally or antidiagonally adjacent
 *            neighbours, with upper‑left cell fixed to 0
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   check_center
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a transfer‑matrix DP over pairs of consecutive columns. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/check_center.h"

static bool tables_built = false;
static bool start_ok[32][32];
static unsigned char trans_cnt[32][32];
static int trans_next[32][32][32];
static bool final_ok[32][32];

/* Build auxiliary tables once. */
static void build_tables(void)
{
    if (tables_built) return;
    tables_built = true;

    /* Initialize start_ok: first column must have top bit 0. */
    for (int p = 0; p < 32; ++p) {
        for (int q = 0; q < 32; ++q) {
            start_ok[p][q] = false;
        }
    }
    for (int p = 0; p < 32; ++p) {
        if ((p & 1) != 0) continue;           /* top of first column must be 0 */
        for (int q = 0; q < 32; ++q) {
            start_ok[p][q] = check_center(false, true, 0, p, q);
        }
    }

    for (int p = 0; p < 32; ++p) {
        for (int q = 0; q < 32; ++q) {
            final_ok[p][q] = check_center(true, false, p, q, 0);
        }
    }

    for (int p = 0; p < 32; ++p) {
        for (int q = 0; q < 32; ++q) {
            unsigned char c = 0;
            for (int r = 0; r < 32; ++r) {
                if (check_center(true, true, p, q, r)) {
                    trans_next[p][q][c++] = r;
                }
            }
            trans_cnt[p][q] = c;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined below offset 1 */
    build_tables();

    /* n == 1: only one column, top bit must be 0, no neighbours. */
    if (n == 1) {
        seq_t ans = 0;
        for (int c = 0; c < 32; ++c)
            if ((c & 1) == 0) ans += 1;
        return ans;                  /* 16 */
    }

    /* n == 2: two columns, left column top bit 0. */
    if (n == 2) {
        seq_t ans = 0;
        for (int c1 = 0; c1 < 32; ++c1) {
            if ((c1 & 1) != 0) continue;           /* top of col1 is 0 */
            for (int c2 = 0; c2 < 32; ++c2) {
                if (check_center(false, true, 0, c1, c2) &&
                    check_center(true, false, c1, c2, 0))
                    ans += 1;
            }
        }
        return ans;                  /* 21 */
    }

    /* n >= 3: transfer‑matrix DP over consecutive column pairs. */
    static seq_t cur[32][32];
    static seq_t nxt[32][32];

    /* initialise cur with admissible first two columns. */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            cur[i][j] = 0;
    for (int p = 0; p < 32; ++p) {
        if ((p & 1) != 0) continue;              /* top of first column is 0 */
        for (int q = 0; q < 32; ++q)
            if (start_ok[p][q])
                cur[p][q] = 1;
    }

    /* perform n-2 transitions (each adds one new column). */
    for (idx_t step = 1; step <= n - 2; ++step) {
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                nxt[i][j] = 0;
        for (int p = 0; p < 32; ++p) {
            for (int q = 0; q < 32; ++q) {
                seq_t val = cur[p][q];
                if (val == 0) continue;
                unsigned char cnt = trans_cnt[p][q];
                for (unsigned char t = 0; t < cnt; ++t) {
                    int r = trans_next[p][q][t];
                    nxt[q][r] += val;
                }
            }
        }
        /* swap cur and nxt */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                cur[i][j] = nxt[i][j];
    }

    /* sum over final states where last column satisfies the rule. */
    seq_t ans = 0;
    for (int p = 0; p < 32; ++p)
        for (int q = 0; q < 32; ++q)
            if (final_ok[p][q])
                ans += cur[p][q];
    return ans;
}
