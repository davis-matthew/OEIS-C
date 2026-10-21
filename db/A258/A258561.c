/*
 * A258561  Number of (n+2)X(1+2) 0..2 arrays with no 3x3 subblock diagonal
 *          sum equal to the antidiagonal sum or central row sum equal to the
 *          central column sum.
 * https://oeis.org/A258561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 13, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A258561 – Count of (n+2)×3 arrays over {0,1,2} in which every 3×3 subblock
    satisfies
        1) diagonal sum ≠ antidiagonal sum        and
        2) central row sum ≠ central column sum.
    The array has height m = n+2 rows and width 3 columns.
    The counting is performed with a dynamic programme whose
    state are the last two rows of the array.
    O(m·27³) ≈ 2·10⁴·m elementary operations – fast enough for n up to 10⁴.
    No caching between calls is used; each call recomputes the dp.
*/

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* --------------------------------------------------------------------
       Pre‑compute row digit tables and the table of allowed triples
       once per translation unit (static local initialization).
       -------------------------------------------------------------------- */
    static int digits[27][3];          /* decimal base‑3 digits of each row  */
    static bool allowed[27][27][27];   /* valid triple a,b,c?                 */
    static bool init = false;          /* flag for one‑time initialisation   */

    if (!init) {
        /* fill digits[0..26] – each row is a 3‑digit base‑3 number */
        for (int r = 0; r < 27; ++r) {
            int tmp = r;
            digits[r][2] = tmp % 3;     tmp /= 3;
            digits[r][1] = tmp % 3;     tmp /= 3;
            digits[r][0] = tmp % 3;
        }
        /* Determine which triples are allowed                           */
        for (int a = 0; a < 27; ++a)
            for (int b = 0; b < 27; ++b)
                for (int c = 0; c < 27; ++c) {
                    int diag     = digits[a][0] + digits[b][1] + digits[c][2];
                    int antidiag = digits[a][2] + digits[b][1] + digits[c][0];
                    if (diag == antidiag) {                     /* 1st rule  */
                        allowed[a][b][c] = false;
                        continue;
                    }
                    int rowSum = digits[a][0] + digits[a][1] + digits[a][2];
                    int colSum = digits[b][1] + digits[b][1] + digits[b][1];
                    /* Wait: central row is row b, central column is index 1 */
                    int centralRow = digits[b][0] + digits[b][1] + digits[b][2];
                    int centralCol = digits[a][1] + digits[b][1] + digits[c][1];
                    allowed[a][b][c] = (centralRow != centralCol);
                }
        init = true;
    }

    /* --------------------------------------------------------------
       Dynamic programming over the rows.
       dp[s1][s2] – number of ways to obtain a prefix whose last two
       rows are s1 (row i‑1) and s2 (row i).
       -------------------------------------------------------------- */
    typedef struct {
        seq_t a[27][27];
    } dp_block_t;
    dp_block_t dp0, dp1;     /* two matrices for toggling              */
    dp_block_t *dp   = &dp0;
    dp_block_t *next = &dp1;

    /* initialise dp for first two rows – all 27×27 prefixes count 1 */
    for (int i = 0; i < 27; ++i)
        for (int j = 0; j < 27; ++j)
            dp->a[i][j] = 1;

    /* the array has m = n+2 rows; we already accounted for two rows  */
    idx_t m = n + 2;
    idx_t steps = m - 2;            /* number of additional rows to place */

    for (idx_t s = 0; s < steps; ++s) {

        /* clear the next matrix */
        for (int i = 0; i < 27; ++i)
            for (int j = 0; j < 27; ++j)
                next->a[i][j] = 0;

        /* transition */
        for (int a = 0; a < 27; ++a)
            for (int b = 0; b < 27; ++b) {
                seq_t val = dp->a[a][b];
                if (val == 0) continue;
                for (int c = 0; c < 27; ++c)
                    if (allowed[a][b][c])
                        next->a[b][c] += val;
            }

        /* swap matrices for next iteration */
        dp   = dp   == &dp0 ? &dp1 : &dp0;
        next = next == &dp1 ? &dp0 : &dp1;
    }

    /* sum all ways for the last two rows – gives the total count   */
    seq_t result = 0;
    for (int a = 0; a < 27; ++a)
        for (int b = 0; b < 27; ++b)
            result += dp->a[a][b];

    return result;
}
