/*
 * A295036  Number of nX4 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 1 or 2 1s.
 * https://oeis.org/A295036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A295036 - number of n×4 binary arrays where each 1 has * 1 or 2
 *            horizontally/vertically adjacent 1s. * Computed by dynamic
 *            programming over row patterns of width 4. * The state is the
 *            last two rows, allowing us to validate a row * when the next row
 *            is chosen. * * Complexity: O(n * 16³) ≈ O(n) wit...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define PAT 16               /* number of patterns for a row (2⁴) */

/* Pre–compute validity of a pattern when its up and down neighbors are known. */
static bool valid[PAT][PAT][PAT];

static void init_valid(void)
{
    for (int up = 0; up < PAT; ++up)
        for (int mid = 0; mid < PAT; ++mid)
            for (int down = 0; down < PAT; ++down) {
                bool ok = true;
                for (int pos = 0; pos < 4; ++pos) {
                    if (mid & (1<<pos)) {
                        int cnt = 0;
                        if (pos > 0 && (mid & (1<<(pos-1)))) ++cnt;          /* left */
                        if (pos < 3 && (mid & (1<<(pos+1)))) ++cnt;          /* right */
                        if (up  & (1<<pos)) ++cnt;                          /* up   */
                        if (down& (1<<pos)) ++cnt;                          /* down */
                        if (cnt != 1 && cnt != 2) { ok = false; break; }
                    }
                }
                valid[up][mid][down] = ok;
            }
}

/* counts the number of n×4 arrays with the adjacency property.
 *   n >= 1
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* 0 not used, but guard */
    /* initialise validity table once (static init is lazy, use guard) */
    static bool inited = false;
    if (!inited) { init_valid(); inited = true; }

    /* n == 1 : no up or down neighbour */
    if (n == 1) {
        seq_t total = 0;
        for (int pat = 0; pat < PAT; ++pat)
            if (valid[0][pat][0]) total += 1;
        return total;
    }

    /* dp[prev][cur] = number of ways to reach current state */
    seq_t dp[PAT][PAT] = {0};
    seq_t ndp[PAT][PAT] = {0};

    /* initialise with first two rows */
    for (int p1 = 0; p1 < PAT; ++p1)
        for (int p2 = 0; p2 < PAT; ++p2)
            if (valid[0][p1][p2]) dp[p1][p2] = 1;

    /* process rows 3..n */
    for (idx_t row = 3; row <= n; ++row) {
        /* clear ndp */
        for (int i = 0; i < PAT; ++i)
            for (int j = 0; j < PAT; ++j)
                ndp[i][j] = 0;

        for (int prev2 = 0; prev2 < PAT; ++prev2)
            for (int prev = 0; prev < PAT; ++prev) {
                seq_t ways = dp[prev2][prev];
                if (!ways) continue;
                for (int cur = 0; cur < PAT; ++cur)
                    if (valid[prev2][prev][cur])
                        ndp[prev][cur] += ways;
            }
        /* move ndp to dp */
        for (int i = 0; i < PAT; ++i)
            for (int j = 0; j < PAT; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* sum over states where the last row is valid (no down neighbour) */
    seq_t result = 0;
    for (int up = 0; up < PAT; ++up)
        for (int cur = 0; cur < PAT; ++cur)
            if (valid[up][cur][0])
                result += dp[up][cur];

    return result;
}
