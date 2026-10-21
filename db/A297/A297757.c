/*
 * A297757  Number of nX3 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 2, 3 or 4 neighboring 1s.
 * https://oeis.org/A297757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 binary arrays where every 1 has 2–4
 *            neighboring 1s among horizontal, diagonal, and antidiagonal
 *            positions (8‑neighborhood without vertical adjacency). Computed
 *            by dynamic programming over rows
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool row_valid(uint8_t above, uint8_t row, uint8_t below)
{
    for (int c = 0; c < 3; ++c) {
        if (!(row & (1u << c))) continue;        /* skip 0 cells */
        int cnt = 0;
        if (c > 0 && (row & (1u << (c - 1)))) ++cnt;   /* left */
        if (c < 2 && (row & (1u << (c + 1)))) ++cnt;   /* right */
        if (c > 0 && (above & (1u << (c - 1)))) ++cnt;/* upper‑left */
        if (c < 2 && (above & (1u << (c + 1)))) ++cnt;/* upper‑right */
        if (c > 0 && (below & (1u << (c - 1)))) ++cnt; /* lower‑left */
        if (c < 2 && (below & (1u << (c + 1)))) ++cnt; /* lower‑right */
        if (cnt < 2 || cnt > 4) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;  /* only all‑zero array satisfies */

    /* Precompute top and bottom validation tables */
    static bool goodTop[8][8];
    static bool goodBottom[8][8];
    static bool init_done = false;
    if (!init_done) {
        for (int m = 0; m < 8; ++m)
            for (int p = 0; p < 8; ++p) {
                goodTop[m][p] = row_valid(0, (uint8_t)m, (uint8_t)p);
                goodBottom[m][p] = row_valid((uint8_t)m, (uint8_t)p, 0);
            }
        init_done = true;
    }

    /* DP arrays for current and next states: pair of last two rows */
    seq_t dp[8][8] = {0};
    seq_t next[8][8] = {0};

    /* Initialize with first two rows */
    for (int r0 = 0; r0 < 8; ++r0)
        for (int r1 = 0; r1 < 8; ++r1)
            if (goodTop[r0][r1])
                dp[r0][r1] = (seq_t)1;

    /* Add remaining rows (from 3rd row to n-th row) */
    for (idx_t row = 2; row < n; ++row) {          /* row index starts at 0 */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                next[i][j] = (seq_t)0;

        for (int prev = 0; prev < 8; ++prev)
            for (int cur = 0; cur < 8; ++cur) {
                seq_t ways = dp[prev][cur];
                if (ways == 0) continue;
                for (int nxt = 0; nxt < 8; ++nxt) {
                    if (row_valid((uint8_t)prev, (uint8_t)cur, (uint8_t)nxt))
                        next[cur][nxt] += ways;
                }
            }
        /* swap dp and next */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp[i][j] = next[i][j];
    }

    /* Final validation of the last row */
    seq_t ans = (seq_t)0;
    for (int prev = 0; prev < 8; ++prev)
        for (int cur = 0; cur < 8; ++cur)
            if (goodBottom[prev][cur])
                ans += dp[prev][cur];

    return ans;
}
