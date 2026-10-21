/*
 * A297649  Number of nX3 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 1, 2 or 3 neighboring 1s.
 * https://oeis.org/A297649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A297649:
   Number of n×3 0/1 arrays where every 1 has 1–3 neighbours
   among the six cells that are horizontally or diagonally
   adjacent (left/right, up‑left/up‑right, down‑left/down‑right). */

#include <stdbool.h>

/* Pre‑computed table:
   valid[a][b][c] == true  ⇔  every 1 in row b has 1–3 neighbours
   in rows a (above), b (middle), c (below), using only horizontal
   and diagonal adjacency (vertical adjacency excluded). */
static bool valid[8][8][8];
static bool table_ready = false;

static void init_valid(void)
{
    for (int a = 0; a < 8; ++a) {
        for (int b = 0; b < 8; ++b) {
            for (int c = 0; c < 8; ++c) {
                bool ok = true;
                for (int j = 0; j < 3 && ok; ++j) {
                    if ((b >> j) & 1) {            /* a 1 in middle row */
                        int cnt = 0;
                        /* horizontal neighbours in the same row */
                        if (j > 0 && (b >> (j - 1)) & 1) ++cnt;
                        if (j + 1 < 3 && (b >> (j + 1)) & 1) ++cnt;
                        /* diagonal neighbours from the row above */
                        if (j > 0 && (a >> (j - 1)) & 1) ++cnt;
                        if (j + 1 < 3 && (a >> (j + 1)) & 1) ++cnt;
                        /* diagonal neighbours from the row below */
                        if (j > 0 && (c >> (j - 1)) & 1) ++cnt;
                        if (j + 1 < 3 && (c >> (j + 1)) & 1) ++cnt;
                        /* check the required range 1–3 */
                        if (cnt < 1 || cnt > 3) ok = false;
                    }
                }
                valid[a][b][c] = ok;
            }
        }
    }
    table_ready = true;
}

seq_t a(idx_t n)
{
    if (!table_ready) init_valid();

    if (n == 0) return 0;              /* by convention */

    /* dp[prev][curr] – number of ways ending with rows (prev, curr) */
    seq_t dp[8][8] = {{0}};
    seq_t ndp[8][8];

    /* first row: above is the zero row */
    for (int r = 0; r < 8; ++r) dp[0][r] = 1;

    for (idx_t step = 1; step < n; ++step) {
        /* reset ndp */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                ndp[i][j] = 0;

        for (int prev = 0; prev < 8; ++prev) {
            for (int curr = 0; curr < 8; ++curr) {
                seq_t ways = dp[prev][curr];
                if (!ways) continue;
                for (int nxt = 0; nxt < 8; ++nxt) {
                    if (valid[prev][curr][nxt])
                        ndp[curr][nxt] += ways;
                }
            }
        }
        /* copy ndp to dp for next iteration */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* last row must be valid with a zero row below */
    seq_t total = 0;
    for (int prev = 0; prev < 8; ++prev) {
        for (int curr = 0; curr < 8; ++curr) {
            if (valid[prev][curr][0])
                total += dp[prev][curr];
        }
    }
    return total;
}
