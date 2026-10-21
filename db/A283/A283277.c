/*
 * A283277  Number of n X 3 0..1 arrays with no 1 equal to more than two of
 *          its horizontal, vertical and antidiagonal neighbors.
 * https://oeis.org/A283277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of n×3 binary arrays in which a cell
 *            containing 1 is not equal * to more than two of its horizontal,
 *            vertical, and antidiagonal neighbours. * * Computation: Dynamic
 *            programming over rows. Each state consists of the last two rows.
 *            * When a new row is added we check that the midd...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Internal table: valid[up][mid][down] is true iff the middle 3‑bit pattern 'mid'
 * satisfies the neighbour rule when its above neighbour is 'up' and below neighbour
 * is 'down'.  All rows are 3‑bit values in [0,7]. */
static bool valid[8][8][8];
static bool valid_computed = false;

/* Build the validity table once. */
static void compute_valid(void)
{
    if (valid_computed) return;
    valid_computed = true;

    for (int up = 0; up < 8; ++up) {
        for (int mid = 0; mid < 8; ++mid) {
            for (int down = 0; down < 8; ++down) {
                bool ok = true;
                for (int j = 0; j < 3 && ok; ++j) {
                    if ((mid >> j) & 1) {        /* cell (mid, j) is a 1 */
                        int cnt = 0;

                        /* horizontal neighbours */
                        if (j > 0 && (mid >> (j - 1)) & 1) ++cnt;
                        if (j < 2 && (mid >> (j + 1)) & 1) ++cnt;

                        /* vertical neighbours */
                        if ((up >> j) & 1)   ++cnt;
                        if ((down >> j) & 1) ++cnt;

                        /* antidiagonal neighbours */
                        if (j < 2 && (up >> (j + 1)) & 1)   ++cnt;
                        if (j > 0 && (down >> (j - 1)) & 1) ++cnt;

                        if (cnt > 2) {
                            ok = false;
                        }
                    }
                }
                valid[up][mid][down] = ok;
            }
        }
    }
}

/*--------------------------------------------------------------------
 * Main sequence function
 *--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* The DP array holds counts for pairs of consecutive rows.
       dp[prev][cur] = number of ways to fill the processed rows
       whose last two rows are 'prev' and 'cur'. */
    seq_t dp[8][8];
    seq_t ndp[8][8];

    compute_valid();

    /* Initialise DP: processed zero rows, the "above" row is all zeros. */
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            dp[i][j] = 0;

    /* After the first real row, we have not yet verified it, so we
       simply record the row.  The fictitious row above is 0. */
    for (int r = 0; r < 8; ++r)
        dp[0][r] = 1;

    /* Process rows 2..n if n>=2. */
    for (idx_t row = 2; row <= n; ++row) {
        /* zero ndp */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                ndp[i][j] = 0;

        for (int up = 0; up < 8; ++up) {
            for (int mid = 0; mid < 8; ++mid) {
                seq_t ways = dp[up][mid];
                if (!ways) continue;
                for (int down = 0; down < 8; ++down) {
                    if (!valid[up][mid][down]) continue;
                    ndp[mid][down] += ways;
                }
            }
        }

        /* copy ndp to dp for next iteration */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* After all rows processed, the last row (below row n) must be all zeros. */
    seq_t result = 0;
    if (n == 1) {
        /* Special case: only one row, neighbours above and below are zero. */
        for (int mid = 0; mid < 8; ++mid) {
            if (valid[0][mid][0]) {
                result += 1;
            }
        }
    } else {
        for (int up = 0; up < 8; ++up) {
            for (int mid = 0; mid < 8; ++mid) {
                seq_t ways = dp[up][mid];
                if (!ways) continue;
                if (valid[up][mid][0]) {
                    result += ways;
                }
            }
        }
    }

    return result;
}
