/*
 * A296400  Number of nX3 0..1 arrays with each 1 horizontally, vertically or
 *          antidiagonally adjacent to 2 or 3 neighboring 1s.
 * https://oeis.org/A296400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A296400: Number of n×3 0..1 arrays with each 1 horizontally, vertically or
 * antidiagonally adjacent to 2 or 3 neighboring 1s.
 *
 * The adjacency set consists of the four orthogonal neighbours (left, right,
 * up, down) plus the two anti‑diagonal neighbours in the (up‑right / down‑left)
 * direction.  For each 1-cell the number of such neighbouring 1‑cells must be
 * exactly 2 or 3.
 */

#include <stdbool.h>

#define PAT 8 /* there are 2^3 = 8 possible 3‑bit rows */

/* Precomputed table of validity: valid[prev][curr][next] is true iff every 1 in
 * the middle row 'curr' has 2 or 3 neighbours when the row above is 'prev'
 * and the row below is 'next'.
 */
static bool valid[PAT][PAT][PAT];
static bool valid_inited = false;

static void init_valid(void)
{
    for (int a = 0; a < PAT; ++a) {
        for (int b = 0; b < PAT; ++b) {
            for (int c = 0; c < PAT; ++c) {
                bool ok = true;
                for (int j = 0; j < 3; ++j) {
                    if ((b >> j) & 1) { /* 1 in middle row */
                        int ncount = 0;
                        /* horizontal neighbours */
                        if (j > 0)   ncount += (b >> (j-1)) & 1;
                        if (j < 2)   ncount += (b >> (j+1)) & 1;
                        /* vertical neighbours */
                        ncount += (a >> j) & 1; /* up */
                        ncount += (c >> j) & 1; /* down */
                        /* anti‑diagonal neighbours (up‑right / down‑left) */
                        if (j < 2)   ncount += (a >> (j+1)) & 1; /* up‑right */
                        if (j > 0)   ncount += (c >> (j-1)) & 1; /* down‑left */
                        if (!(ncount == 2 || ncount == 3)) {
                            ok = false;
                            break;
                        }
                    }
                }
                valid[a][b][c] = ok;
            }
        }
    }
    valid_inited = true;
}

/* Count arrays of size n×3 that satisfy the adjacency requirement. */
seq_t a(idx_t n)
{
    if (!valid_inited) init_valid();

    if (n == 0) return 0;           /* offset is 1; 0 is undefined */

    /* dp[prev][curr] – number of ways after the current row (idx curr) has
     * been processed, where 'prev' is the row above. */
    seq_t dp[PAT][PAT];
    seq_t next[PAT][PAT];

    for (int i = 0; i < PAT; ++i)
        for (int j = 0; j < PAT; ++j)
            dp[i][j] = 0;

    /* first real row: prev is dummy 0‑row. */
    for (int p = 0; p < PAT; ++p)
        dp[0][p] = 1;

    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < PAT; ++i)
            for (int j = 0; j < PAT; ++j)
                next[i][j] = 0;

        for (int prev = 0; prev < PAT; ++prev) {
            for (int curr = 0; curr < PAT; ++curr) {
                seq_t ways = dp[prev][curr];
                if (ways == 0) continue;
                for (int nxt = 0; nxt < PAT; ++nxt) {
                    if (valid[prev][curr][nxt]) {
                        next[curr][nxt] = next[curr][nxt] + ways;
                    }
                }
            }
        }

        for (int i = 0; i < PAT; ++i)
            for (int j = 0; j < PAT; ++j)
                dp[i][j] = next[i][j];
    }

    /* final dummy bottom row of zeros */
    seq_t ans = 0;
    for (int prev = 0; prev < PAT; ++prev) {
        for (int curr = 0; curr < PAT; ++curr) {
            if (valid[prev][curr][0]) {
                ans = ans + dp[prev][curr];
            }
        }
    }
    return ans;
}
