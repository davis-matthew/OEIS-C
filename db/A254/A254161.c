/*
 * A254161  Number of (n+2)X(1+2) 0..1 arrays with every 3X3 subblock diagonal
 *          maximum minus antidiagonal median nondecreasing horizontally,
 *          vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A254161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 11, u64 n <= 27, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) computed by dynamic programming.
   Count the (n+2)×3 0‑1 arrays for which, for every vertical
   shift of the 3×3 subblock, the value
          (max of the main diagonal) – (median of the anti‑diagonal)
   is non‑decreasing. */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    size_t m = (size_t)n + 2;          /* number of rows in the array */

    /* state: last two rows (prev, cur) and last f value
       (-1, 0, 1) indexed as 0,1,2. */
    seq_t curr[8][8][3] = {};

    /* initialise: before any 3×3 block has appeared, last f is -1 */
    for (int r1 = 0; r1 < 8; ++r1)
        for (int r2 = 0; r2 < 8; ++r2)
            curr[r1][r2][0] = 1;      /* count = 1 for each pair of rows */

    /* pre‑compute f for all triples of rows */
    static int fval[8][8][8];
    static int finit = 0;
    if (!finit) {
        for (int a = 0; a < 8; ++a)
            for (int b = 0; b < 8; ++b)
                for (int c = 0; c < 8; ++c) {
                    int dmax = ((a & 1) | ((b >> 1) & 1) | ((c >> 2) & 1));
                    int maj  = (((a >> 2) & 1) + ((b >> 1) & 1) + (c & 1) >= 2);
                    fval[a][b][c] = dmax - maj;   /* -1, 0, or 1 */
                }
        finit = 1;
    }

    seq_t next[8][8][3];

    for (size_t row = 3; row <= m; ++row) {   /* add one row at a time */
        /* zero the next array */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                for (int k = 0; k < 3; ++k)
                    next[i][j][k] = 0;

        for (int p = 0; p < 8; ++p) {
            for (int q = 0; q < 8; ++q) {
                for (int fi = 0; fi < 3; ++fi) {
                    seq_t cnt = curr[p][q][fi];
                    if (cnt == 0) continue;
                    for (int r = 0; r < 8; ++r) {
                        int new_f = fval[p][q][r] + 1;   /* index 0..2 */
                        if (new_f < fi) continue;       /* must be nondecreasing */
                        next[q][r][new_f] += cnt;
                    }
                }
            }
        }

        /* copy next to curr for the next iteration */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                for (int k = 0; k < 3; ++k)
                    curr[i][j][k] = next[i][j][k];
    }

    seq_t result = 0;
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            for (int k = 0; k < 3; ++k)
                result += curr[i][j][k];

    return result;
}
