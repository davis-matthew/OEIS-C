/*
 * A297716  Number of nX4 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 1, 2 or 4 neighboring 1s.
 * https://oeis.org/A297716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of n×4 0–1 arrays in which each 1 has 1, 2 or
 *            4 neighbouring 1’s horizontally, diagonally or antidiagonally.
 *            Computed by dynamic programming over rows
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Transition validity table:
       valid[prev][cur][next] == true iff every 1 in the middle row 'cur'
       has 1, 2 or 4 neighbouring 1’s given rows 'prev' (above) and
       'next' (below).  Zero rows are represented by the pattern 0.  */
    static bool valid[16][16][16];
    static bool initialized = false;

    if (!initialized) {
        for (int prev = 0; prev < 16; ++prev) {
            for (int cur  = 0; cur  < 16; ++cur) {
                for (int next = 0; next < 16; ++next) {
                    bool ok = true;
                    for (int j = 0; j < 4; ++j)                /* column j (0‑based) */
                        if (cur & (1u << j)) {                /* a 1 at (row, j) */
                            int cnt = 0;
                            /* same‑row neighbours */
                            if (j > 0          && (cur >> (j-1)) & 1) ++cnt; /* left  */
                            if (j < 3          && (cur >> (j+1)) & 1) ++cnt; /* right */
                            /* diagonal neighbours from the row above (prev) */
                            if (j > 0          && (prev >> (j-1)) & 1) ++cnt; /* up‑left  */
                            if (j < 3          && (prev >> (j+1)) & 1) ++cnt; /* up‑right */
                            /* antidiagonal neighbours from the row below (next) */
                            if (j > 0          && (next >> (j-1)) & 1) ++cnt; /* down‑left  */
                            if (j < 3          && (next >> (j+1)) & 1) ++cnt; /* down‑right */
                            if (!(cnt==1 || cnt==2 || cnt==4)) { ok = false; break; }
                        }
                    valid[prev][cur][next] = ok;
                }
            }
        }
        initialized = true;
    }

    /* dp[prev][cur] counts partial arrays whose last two rows are
       'prev' and then 'cur'.  All calculations are performed in seq_t. */
    seq_t dp[16][16]  = {0}, newdp[16][16] = {0};

    /* First row: preceding row is the dummy zero row.  We store
       a pair (0, cur) with weight 1 for every possible pattern. */
    for (int cur = 0; cur < 16; ++cur)
        dp[0][cur] = 1;

    /* Build the remaining rows, if any.  For each new row we check
       that the middle row satisfies the adjacency rule with its
       neighbours above and below. */
    for (idx_t step = 1; step < n; ++step) {
        /* reset newdp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                newdp[i][j] = 0;

        for (int prev = 0; prev < 16; ++prev) {
            for (int cur  = 0; cur  < 16; ++cur) {
                seq_t val = dp[prev][cur];
                if (!val) continue;
                for (int nxt = 0; nxt < 16; ++nxt)
                    if (valid[prev][cur][nxt])
                        newdp[cur][nxt] += val;
            }
        }

        /* copy newdp into dp for the next iteration */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* Last row must be adjacent to a zero row below.  Sum all
       partial arrays whose final pair (prev, cur) satisfies the
       rule with next row 0. */
    seq_t ans = 0;
    for (int prev = 0; prev < 16; ++prev)
        for (int cur = 0; cur < 16; ++cur)
            if (valid[prev][cur][0])
                ans += dp[prev][cur];

    return ans;
}
