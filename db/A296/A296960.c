/*
 * A296960  Number of nX5 0..1 arrays with each 1 adjacent to 2, 3 or 5
 *          king-move neighboring 1s.
 * https://oeis.org/A296960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays in which every 1 has exactly
 *            2, 3 or 5 king‑move neighboring 1s. The array is counted for n ≥
 *            1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 209 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline int count_neighbors(int a, int b, int c, int j)
{
    int cnt = 0;
    /* same row (b) */
    if (j > 0 && (b >> (j - 1)) & 1) cnt++;
    if (j < 4 && (b >> (j + 1)) & 1) cnt++;
    /* above row (a) */
    if ((a >> j) & 1) cnt++;
    if (j > 0 && (a >> (j - 1)) & 1) cnt++;
    if (j < 4 && (a >> (j + 1)) & 1) cnt++;
    /* below row (c) */
    if ((c >> j) & 1) cnt++;
    if (j > 0 && (c >> (j - 1)) & 1) cnt++;
    if (j < 4 && (c >> (j + 1)) & 1) cnt++;
    return cnt;
}

static inline int valid_trip(int a, int b, int c)
{
    for (int j = 0; j < 5; ++j) {
        if ((b >> j) & 1) {
            int n = count_neighbors(a, b, c, j);
            if (!(n == 2 || n == 3 || n == 5)) return 0;
        }
    }
    return 1;
}

static uint32_t valid_mask[32][32];
static int precomputed = 0;

static void precompute(void)
{
    for (int a = 0; a < 32; ++a) {
        for (int b = 0; b < 32; ++b) {
            uint32_t mask = 0;
            for (int c = 0; c < 32; ++c)
                if (valid_trip(a, b, c))
                    mask |= 1u << c;
            valid_mask[a][b] = mask;
        }
    }
    precomputed = 1;
}

seq_t a(idx_t n)
{
    if (!precomputed) precompute();

    static seq_t dp[32][32];
    static seq_t ndp[32][32];
    /* initialize dp for the first row (row index 1) */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = 0;
    for (int b = 0; b < 32; ++b)
        dp[0][b] = 1;   /* (prev row 0, current row b) */

    /* process rows 2 .. n */
    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                ndp[i][j] = 0;

        for (int a = 0; a < 32; ++a) {
            for (int b = 0; b < 32; ++b) {
                if (dp[a][b] == 0) continue;
                uint32_t mask = valid_mask[a][b];
                while (mask) {
                    int c = __builtin_ctz((unsigned)mask);
                    ndp[b][c] += dp[a][b];
                    mask &= mask - 1;
                }
            }
        }
        /* copy ndp to dp for next iteration */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* finalize by validating the last row with a trailing zero row */
    seq_t ans = 0;
    for (int a = 0; a < 32; ++a) {
        for (int b = 0; b < 32; ++b) {
            if (dp[a][b] == 0) continue;
            if (valid_trip(a, b, 0))
                ans += dp[a][b];
        }
    }
    return ans;
}
