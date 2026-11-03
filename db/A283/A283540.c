/*
 * A283540  Number of n X 5 0..1 arrays with no 1 equal to more than one of
 *          its horizontal, diagonal and antidiagonal neighbors.
 * https://oeis.org/A283540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays where each 1 has at most one
 *            * horizontal or diagonal neighbor with value 1. The function
 *            computes the * sequence by dynamic programming over rows, using
 *            32 possible row masks
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 200 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Test whether a 1 in a row 'row' has at most one 1‑neighbour among
 * horizontal (left/right) and diagonal (up-left/up-right/down-left/down-right)
 * positions specified by the masks. */
static bool valid_mask(int row, int up, int down)
{
    for (int j = 0; j < 5; ++j) {
        if (!((row >> j) & 1))
            continue;                 /* no 1 at this position */

        int cnt = 0;
        /* horizontal neighbors */
        if (j > 0 && ((row >> (j-1)) & 1)) ++cnt;
        if (j < 4 && ((row >> (j+1)) & 1)) ++cnt;
        /* diagonal neighbors (up) */
        if (j > 0 && ((up >> (j-1)) & 1)) ++cnt;
        if (j < 4 && ((up >> (j+1)) & 1)) ++cnt;
        /* diagonal neighbors (down) */
        if (j > 0 && ((down >> (j-1)) & 1)) ++cnt;
        if (j < 4 && ((down >> (j+1)) & 1)) ++cnt;

        if (cnt > 1)
            return false;
    }
    return true;
}

/* Pre‑computed table: next_valid[up][mid][next] == true if the 1's in
 * 'mid' satisfy the rule when the row above is 'up' and the row below
 * will be 'next'. */
static bool next_valid[32][32][32];
static bool initialized = false;

static void init_next_valid(void)
{
    if (initialized)
        return;
    for (int up = 0; up < 32; ++up)
        for (int mid = 0; mid < 32; ++mid)
            for (int next = 0; next < 32; ++next)
                next_valid[up][mid][next] = valid_mask(mid, up, next);
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    init_next_valid();

    static seq_t dp[32][32];
    static seq_t dp_next[32][32];

    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = 0;
    dp[0][0] = 1;          /* before any rows: dummy row 0 and -1 */

    for (idx_t row = 1; row <= n; ++row) {
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp_next[i][j] = 0;

        for (int up = 0; up < 32; ++up) {
            for (int mid = 0; mid < 32; ++mid) {
                seq_t cur = dp[up][mid];
                if (cur == 0)
                    continue;
                for (int down = 0; down < 32; ++down) {
                    if (!next_valid[up][mid][down])
                        continue;
                    dp_next[mid][down] += cur;
                }
            }
        }

        /* move to next state */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = dp_next[i][j];
    }

    /* Final check for the last row (row n) with the imaginary row below being 0 */
    seq_t result = 0;
    for (int up = 0; up < 32; ++up) {
        for (int mid = 0; mid < 32; ++mid) {
            seq_t cur = dp[up][mid];
            if (cur == 0)
                continue;
            if (valid_mask(mid, up, 0))   /* last row constraint, neighbours down are 0 */
                result += cur;
        }
    }

    return result;
}
