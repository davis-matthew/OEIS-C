/*
 * A295939  Number of n X 4 0..1 arrays with each 1 adjacent to 1 or 3
 *          king-move neighboring 1's.
 * https://oeis.org/A295939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of n×4 binary arrays where each 1 has 1 or 3 king‑move *
 *            neighboring 1s. Computed by a transfer‑matrix dynamic
 *            programming * over the rows of the array. The program handles
 *            any n ≥ 1 in * O(256 * 16 * n) time
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int init_flag = 0;
static int16_t valid[16][16][16];   /* 1 if row 'c' is valid with neighbours p and n */

static int row_valid(uint8_t p, uint8_t c, uint8_t n)
{
    for (int col = 0; col < 4; ++col) {
        if (!(c & (1u << col))) continue;  /* 0‑cell: no condition */
        int neigh = 0;
        /* above row p */
        if (col - 1 >= 0 && (p & (1u << (col - 1)))) neigh++;
        if (p & (1u << col)) neigh++;
        if (col + 1 < 4 && (p & (1u << (col + 1)))) neigh++;
        /* same row c (exclude the cell itself) */
        if (col - 1 >= 0 && (c & (1u << (col - 1)))) neigh++;
        if (col + 1 < 4 && (c & (1u << (col + 1)))) neigh++;
        /* below row n */
        if (col - 1 >= 0 && (n & (1u << (col - 1)))) neigh++;
        if (n & (1u << col)) neigh++;
        if (col + 1 < 4 && (n & (1u << (col + 1)))) neigh++;
        if (neigh != 1 && neigh != 3) return 0;
    }
    return 1;
}

static void build_valid(void)
{
    for (int p = 0; p < 16; ++p)
        for (int c = 0; c < 16; ++c)
            for (int n = 0; n < 16; ++n)
                valid[p][c][n] = row_valid(p, c, n);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (!init_flag) { build_valid(); init_flag = 1; }

    static seq_t dp[16][16], ndp[16][16];
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            dp[i][j] = 0;
    dp[0][0] = 1;  /* initial fake row of zeros */

    for (idx_t row = 0; row < n; ++row) {
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                ndp[i][j] = 0;
        for (int p = 0; p < 16; ++p)
            for (int c = 0; c < 16; ++c) {
                if (dp[p][c] == 0) continue;
                const seq_t cnt = dp[p][c];
                for (int nxt = 0; nxt < 16; ++nxt) {
                    if (!valid[p][c][nxt]) continue;
                    ndp[c][nxt] += cnt;
                }
            }
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = ndp[i][j];
    }

    seq_t ans = 0;
    for (int p = 0; p < 16; ++p)
        for (int c = 0; c < 16; ++c) {
            if (dp[p][c] == 0) continue;
            if (valid[p][c][0])  /* final fake row of zeros below */
                ans += dp[p][c];
        }
    return ans;
}
