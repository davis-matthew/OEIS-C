/*
 * A282334  Number of nX4 0..1 arrays with no 1 equal to more than four of its
 *          king-move neighbors, with the exception of exactly one element.
 * https://oeis.org/A282334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 205 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A282334: a(n) = number of n×4 binary arrays with exactly one 1 that has
 * more than four king‑move neighbors; all other 1's have at most four
 * king‑move neighbors. */
#include <stdbool.h>

static void compute_viol_map(int viol_map[16][16][16]) {
    for (int a = 0; a < 16; ++a) {
        for (int b = 0; b < 16; ++b) {
            for (int c = 0; c < 16; ++c) {
                int v = 0;
                for (int j = 0; j < 4; ++j) {
                    if ((b & (1 << j)) == 0) continue;          /* cell is 0 */
                    int cnt = 0;
                    /* above row */
                    if (j > 0) cnt += (a >> (j - 1)) & 1;
                    cnt += (a >> j) & 1;
                    if (j < 3) cnt += (a >> (j + 1)) & 1;
                    /* below row */
                    if (j > 0) cnt += (c >> (j - 1)) & 1;
                    cnt += (c >> j) & 1;
                    if (j < 3) cnt += (c >> (j + 1)) & 1;
                    /* same row horizontal neighbors */
                    if (j > 0) cnt += (b >> (j - 1)) & 1;
                    if (j < 3) cnt += (b >> (j + 1)) & 1;
                    if (cnt > 4) ++v;
                }
                viol_map[a][b][c] = v;
            }
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* offset starts at 1 */

    /* precompute violation counts for every triple of rows */
    static int viol_map[16][16][16];
    static bool initialized = false;
    if (!initialized) {
        compute_viol_map(viol_map);
        initialized = true;
    }

    /* DP arrays: [prev2][prev1][violations] */
    static seq_t dp[16][16][2];
    static seq_t newdp[16][16][2];
    seq_t (*cur)[16][2]  = dp;
    seq_t (*next)[16][2] = newdp;

    /* initialize DP */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            for (int k = 0; k < 2; ++k)
                cur[i][j][k] = 0;
    cur[0][0][0] = 1;  /* no rows processed yet */

    /* process each row */
    for (idx_t row = 0; row < n; ++row) {
        /* clear next */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                for (int k = 0; k < 2; ++k)
                    next[i][j][k] = 0;

        for (int prev2 = 0; prev2 < 16; ++prev2) {
            for (int prev1 = 0; prev1 < 16; ++prev1) {
                for (int viol = 0; viol < 2; ++viol) {
                    seq_t ways = cur[prev2][prev1][viol];
                    if (ways == 0) continue;
                    for (int curr = 0; curr < 16; ++curr) {
                        int add = viol_map[prev2][prev1][curr];
                        int new_viol = viol + add;
                        if (new_viol > 1) continue;
                        next[prev1][curr][new_viol] += ways;
                    }
                }
            }
        }
        /* swap arrays */
        seq_t (*tmp)[16][2] = cur;
        cur = next;
        next = tmp;
    }

    /* finalize the last (bottom) row */
    seq_t ans = 0;
    for (int prev2 = 0; prev2 < 16; ++prev2) {
        for (int prev1 = 0; prev1 < 16; ++prev1) {
            for (int viol = 0; viol < 2; ++viol) {
                seq_t ways = cur[prev2][prev1][viol];
                if (ways == 0) continue;
                int final_add = viol_map[prev2][prev1][0];   /* below row is all 0 */
                if (viol + final_add == 1)
                    ans += ways;
            }
        }
    }
    return ans;
}
