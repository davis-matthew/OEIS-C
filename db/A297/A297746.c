/*
 * A297746  Number of nX5 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 1, 3 or 4 neighboring 1s.
 * https://oeis.org/A297746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   DP over rows counting 1’s with 6‑adjacent neighbours
 *            (horizontally and diagonally, no vertical). The recursion
 *            iterates over all possible triples of consecutive rows and
 *            enforces the neighbour rule for the middle row
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* table_valid[p2][p1][c] tells whether a triple of rows (p2, p1, c)
   satisfies the condition for the middle row (p1). */
static bool table_valid[32 * 32 * 32];
static bool table_inited = false;

/* Check that every 1 in p1 has 1, 3 or 4 neighbours among the
   six allowed positions: left, right, up‑left, up‑right,
   down‑left and down‑right. */
static bool row_valid(unsigned int p2, unsigned int p1, unsigned int c)
{
    for (int j = 0; j < 5; ++j) {
        if (!(p1 & (1u << j))) continue;          /* skip 0 cells */
        int cnt = 0;
        /* horizontal neighbours */
        if (j > 0   && (p1 & (1u << (j - 1)))) ++cnt;
        if (j < 4   && (p1 & (1u << (j + 1)))) ++cnt;
        /* diagonal neighbours */
        if (j > 0   && (p2 & (1u << (j - 1)))) ++cnt;
        if (j < 4   && (p2 & (1u << (j + 1)))) ++cnt;
        /* antidiagonal neighbours */
        if (j > 0   && (c   & (1u << (j - 1)))) ++cnt;
        if (j < 4   && (c   & (1u << (j + 1)))) ++cnt;
        if (!(cnt == 1 || cnt == 3 || cnt == 4)) return false;
    }
    return true;
}

/* Initialise the validity table once. */
static void init_table(void)
{
    if (table_inited) return;
    for (unsigned int p2 = 0; p2 < 32; ++p2)
        for (unsigned int p1 = 0; p1 < 32; ++p1)
            for (unsigned int c = 0; c < 32; ++c) {
                unsigned int idx = p2 * 1024 + p1 * 32 + c;
                table_valid[idx] = row_valid(p2, p1, c);
            }
    table_inited = true;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */
    init_table();

    static seq_t dp[32][32];
    static seq_t ndp[32][32];

    /* clear dp */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = 0;

    /* first row: prev2 = 0, prev1 = r */
    for (int r = 0; r < 32; ++r)
        dp[0][r] = 1;

    /* process remaining rows */
    for (idx_t step = 2; step <= n; ++step) {
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                ndp[i][j] = 0;
        for (int p2 = 0; p2 < 32; ++p2)
            for (int p1 = 0; p1 < 32; ++p1) {
                seq_t cur = dp[p2][p1];
                if (cur == 0) continue;
                for (int c = 0; c < 32; ++c) {
                    unsigned int idx = p2 * 1024 + p1 * 32 + c;
                    if (!table_valid[idx]) continue;
                    ndp[p1][c] += cur;
                }
            }
        /* copy ndp to dp for next iteration */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* final check: last row against a zero pseudo‑row */
    seq_t result = 0;
    for (int p2 = 0; p2 < 32; ++p2)
        for (int p1 = 0; p1 < 32; ++p1) {
            seq_t cur = dp[p2][p1];
            if (cur == 0) continue;
            unsigned int idx = p2 * 1024 + p1 * 32 + 0;  /* curr = 0 */
            if (!table_valid[idx]) continue;
            result += cur;
        }

    return result;
}
