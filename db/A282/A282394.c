/*
 * A282394  Number of nX3 0..1 arrays with no 1 equal to more than three of
 *          its king-move neighbors.
 * https://oeis.org/A282394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A282394 – Number of n×3 binary arrays such that no 1 has more *
 *            than 3 neighboring 1s in the 8 king‑move directions. Computed by
 *            * dynamic programming over the rows
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Pre‑computed validity table: valid[a][b][c] is true iff a,b,c (rows)
 * satisfy the constraint for row b with neighbouring rows a (above)
 * and c (below).  Rows are 3‑bit patterns from 0 to 7. */
static bool valid[8][8][8];
static bool valid_done = false;

static void compute_valid(void)
{
    for (int a = 0; a < 8; ++a)
    for (int b = 0; b < 8; ++b)
    for (int c = 0; c < 8; ++c) {
        bool ok = true;
        for (int j = 0; j < 3 && ok; ++j) {
            if (((b >> j) & 1) == 0) continue;                /* cell is 0, skip */
            int cnt = 0;
            /* same row neighbours */
            if (j > 0 && ((b >> (j - 1)) & 1)) cnt++;
            if (j < 2 && ((b >> (j + 1)) & 1)) cnt++;
            /* above row neighbours */
            if (j > 0 && ((a >> (j - 1)) & 1)) cnt++;
            if ((a >> j) & 1) cnt++;
            if (j < 2 && ((a >> (j + 1)) & 1)) cnt++;
            /* below row neighbours */
            if (j > 0 && ((c >> (j - 1)) & 1)) cnt++;
            if ((c >> j) & 1) cnt++;
            if (j < 2 && ((c >> (j + 1)) & 1)) cnt++;
            if (cnt > 3) ok = false;
        }
        valid[a][b][c] = ok;
    }
    valid_done = true;
}

static seq_t dp[8][8];
static seq_t ndp[8][8];

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, but guard safely */
    if (!valid_done) compute_valid();

    /* initialise DP with the first row, prev row is dummy 0 */
    for (int i = 0; i < 8; ++i)
    for (int j = 0; j < 8; ++j)
        dp[i][j] = 0;

    for (int r = 0; r < 8; ++r)
        dp[0][r] = 1;              /* first real row r */

    /* iterate over remaining rows */
    for (idx_t row = 1; row < n; ++row) {
        for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            ndp[i][j] = 0;

        for (int prev = 0; prev < 8; ++prev) {
            for (int curr = 0; curr < 8; ++curr) {
                seq_t ways = dp[prev][curr];
                if (ways == 0) continue;
                for (int next = 0; next < 8; ++next) {
                    if (valid[prev][curr][next])
                        ndp[curr][next] += ways;
                }
            }
        }
        /* copy ndp back into dp for next iteration */
        for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            dp[i][j] = ndp[i][j];
    }

    /* final validation for the last row (no row below) */
    seq_t result = 0;
    for (int prev = 0; prev < 8; ++prev)
    for (int curr = 0; curr < 8; ++curr)
        if (valid[prev][curr][0])
            result += dp[prev][curr];

    return result;
}
