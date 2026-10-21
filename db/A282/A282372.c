/*
 * A282372  Number of n X 3 0..1 arrays with no 1 equal to more than four of
 *          its king-move neighbors, with the exception of exactly two
 *          elements.
 * https://oeis.org/A282372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 binary arrays in which every 1 cell has at
 *            most 4 1‑neighbors, except exactly two such cells are allowed to
 *            violate this rule
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Precompute violations for all triples of rows */
    static unsigned char viol[8][8][8];
    static int table_ready = 0;
    if (!table_ready) {
        for (int prev = 0; prev < 8; ++prev) {
            for (int curr = 0; curr < 8; ++curr) {
                for (int next = 0; next < 8; ++next) {
                    int v = 0;
                    for (int j = 0; j < 3; ++j) {
                        if (!(curr & (1 << j))) continue;          /* cell is 0 */
                        int cnt = 0;
                        /* same row left/right */
                        if (j > 0 && (curr & (1 << (j - 1)))) cnt++;
                        if (j < 2 && (curr & (1 << (j + 1)))) cnt++;
                        /* prev row */
                        if (j > 0 && (prev & (1 << (j - 1)))) cnt++;
                        if (prev & (1 << j)) cnt++;
                        if (j < 2 && (prev & (1 << (j + 1)))) cnt++;
                        /* next row */
                        if (j > 0 && (next & (1 << (j - 1)))) cnt++;
                        if (next & (1 << j)) cnt++;
                        if (j < 2 && (next & (1 << (j + 1)))) cnt++;
                        if (cnt > 4) v++;
                    }
                    viol[prev][curr][next] = (unsigned char)v;
                }
            }
        }
        table_ready = 1;
    }

    /* DP arrays */
    static seq_t dp[8][8][3];
    static seq_t dp_next[8][8][3];

    /* Zero dp arrays */
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            for (int k = 0; k < 3; ++k)
                dp[i][j][k] = 0;

    /* Initial state: virtual row 0, first row r0 */
    for (int r0 = 0; r0 < 8; ++r0)
        dp[0][r0][0] = (seq_t)1;

    /* Process rows 1 .. n-1 */
    for (idx_t row = 1; row < n; ++row) {
        /* Zero next dp */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                for (int k = 0; k < 3; ++k)
                    dp_next[i][j][k] = 0;

        for (int prev = 0; prev < 8; ++prev) {
            for (int curr = 0; curr < 8; ++curr) {
                for (int ex = 0; ex < 3; ++ex) {
                    seq_t val = dp[prev][curr][ex];
                    if (!val) continue;
                    for (int next = 0; next < 8; ++next) {
                        int add = viol[prev][curr][next];
                        int nextex = ex + add;
                        if (nextex > 2) continue;
                        dp_next[curr][next][nextex] += val;
                    }
                }
            }
        }

        /* Move dp_next to dp */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                for (int k = 0; k < 3; ++k)
                    dp[i][j][k] = dp_next[i][j][k];
    }

    /* Final evaluation of last row with bottom virtual zeros */
    seq_t ans = 0;
    for (int prev = 0; prev < 8; ++prev) {
        for (int curr = 0; curr < 8; ++curr) {
            for (int ex = 0; ex < 3; ++ex) {
                seq_t val = dp[prev][curr][ex];
                if (!val) continue;
                int add = viol[prev][curr][0];
                int final_ex = ex + add;
                if (final_ex == 2) ans += val;
            }
        }
    }
    return ans;
}
