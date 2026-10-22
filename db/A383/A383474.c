/*
 * A383474  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where A(n,k) is the number of lattice paths from (0,0)
 *          to (n,k) using steps (1,0),(2,0),(3,0),(0,1),(0,2),(0,3).
 * https://oeis.org/A383474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,k) = A(k,n) with * A(n,k) = A(n-1,k) + A(n-2,k) + A(n-3,k) *
 *            + A(n,k-1) + A(n,k-2) + A(n,k-3); * The function below returns
 *            a(n), the sequence obtained by reading * the square array by
 *            antidiagonals downwards. * a(n) = A(i,j) where (i,j) are the
 *            coordinates of n on the * antidiagonals (starti...
 * Range:     u8 n <= 30, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;     /* cached DP table (row-major) */
static size_t dp_size = 0;   /* dimension of the table */

/* convert global index n to coordinates (i,j) on antidiagonals */
static void index_to_coord(idx_t n, idx_t *i, idx_t *j)
{
    /* Find diagonal d such that d(d+1)/2 <= n < (d+1)(d+2)/2 */
    size_t d = 0;
    idx_t cum = 0;
    while (cum + (idx_t)(d + 1) <= n) {
        cum += (idx_t)(d + 1);
        d++;
    }
    *i = n - cum;      /* i runs from 0 to d */
    *j = (idx_t)d - *i;
}

seq_t a(idx_t n)
{
    idx_t i, j;
    index_to_coord(n, &i, &j);          /* coordinates for A(i,j) */

    /* Determine maximum index needed for DP table: max(i,j) */
    size_t max_needed = (i > j) ? (size_t)i : (size_t)j;
    size_t new_size = max_needed + 1;    /* table dimension */

    /* Recompute DP table if it is not large enough */
    if (dp == NULL || dp_size < new_size) {
        /* Free old table if it exists */
        if (dp != NULL) {
            free(dp);
        }

        /* Allocate new table */
        dp = (seq_t *)malloc(new_size * new_size * sizeof(seq_t));
        if (dp == NULL) {
            /* Allocation failed; return 0 as a safe fallback */
            return (seq_t)0;
        }

        /* Compute DP values up to new_size-1 */
        for (size_t a_idx = 0; a_idx < new_size; ++a_idx) {
            for (size_t b_idx = 0; b_idx < new_size; ++b_idx) {
                if (a_idx == 0 && b_idx == 0) {
                    dp[a_idx * new_size + b_idx] = (seq_t)1;
                } else {
                    seq_t sum = (seq_t)0;
                    if (a_idx >= 1) sum += dp[(a_idx - 1) * new_size + b_idx];
                    if (a_idx >= 2) sum += dp[(a_idx - 2) * new_size + b_idx];
                    if (a_idx >= 3) sum += dp[(a_idx - 3) * new_size + b_idx];
                    if (b_idx >= 1) sum += dp[a_idx * new_size + (b_idx - 1)];
                    if (b_idx >= 2) sum += dp[a_idx * new_size + (b_idx - 2)];
                    if (b_idx >= 3) sum += dp[a_idx * new_size + (b_idx - 3)];
                    dp[a_idx * new_size + b_idx] = sum;
                }
            }
        }

        dp_size = new_size;
    }

    return dp[(size_t)i * dp_size + (size_t)j];
}
