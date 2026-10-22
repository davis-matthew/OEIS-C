/*
 * A362563  Triangle T(n, k) read by rows, where T(n, k) is the number of
 *          {123,132}-avoiding parking functions of size n with k active
 *          sites, for 2 <= k <= n+1.
 * https://oeis.org/A362563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle recurrence (OEIS A362563):
 *   T(1,2) = 1
 *   T(2,2) = 1, T(2,3) = 2
 *   For n > 2:
 *      T(n,k) = 2*T(n-1,k-1) + ∑_{j=k-1}^{n-1} T(n-2,j)
 *   Here a(n) is the triangle element read row‑by‑row,
 *   so index n → (row r, column k) where r(r+1)/2 >= n.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row (r) and column (k) that correspond to the
       linear index n in the triangle. */
    idx_t r = 1;
    idx_t cum = 0;
    while (1) {
        cum += r;              /* cumulative elements up to row r */
        if (cum >= n) break;
        ++r;
    }
    idx_t offset = n - (cum - r);      /* 1‑based position within row r */
    idx_t k = 2 + offset - 1;           /* column index in that row */

    /* Allocate space for two previous rows and the current row.
       Arrays are indexed from 0; we use indices 2..r+1. */
    size_t size = (size_t)(r + 3);      /* enough for highest index (r+1) */
    seq_t *row_n2 = (seq_t *)calloc(size, sizeof(seq_t)); /* row n-2 */
    seq_t *row_n1 = (seq_t *)calloc(size, sizeof(seq_t)); /* row n-1 */
    seq_t *row_cur = (seq_t *)calloc(size, sizeof(seq_t));/* row n   */
    if (!row_n2 || !row_n1 || !row_cur) {
        /* Allocation failed – abort. */
        exit(EXIT_FAILURE);
    }

    /* ----- Row 1 ----- */
    row_n2[2] = (seq_t)1;
    if (r == 1) {
        seq_t result = row_n2[k];
        free(row_n2); free(row_n1); free(row_cur);
        return result;
    }

    /* ----- Row 2 ----- */
    row_n1[2] = (seq_t)1;
    row_n1[3] = (seq_t)2;
    if (r == 2) {
        seq_t result = row_n1[k];
        free(row_n2); free(row_n1); free(row_cur);
        return result;
    }

    /* ----- Rows 3 .. r ----- */
    seq_t *suffix = (seq_t *)calloc(r + 2, sizeof(seq_t));
    if (!suffix) {
        free(row_n2); free(row_n1); free(row_cur);
        exit(EXIT_FAILURE);
    }

    for (idx_t cur = 3; cur <= r; ++cur) {
        /* Build suffix sums for the row n-2 (row_n2). */
        seq_t acc = 0;
        for (idx_t j = cur - 1; j >= 2; --j) {
            acc += row_n2[j];
            suffix[j] = acc;
        }
        suffix[1] = acc;   /* T(_,1)=0, but included safely */
        suffix[cur] = 0;   /* empty sum for j = cur */

        /* Compute the current row using the recurrence. */
        for (idx_t kk = 2; kk <= cur + 1; ++kk) {
            seq_t term1 = row_n1[kk - 1];      /* T(n-1,k-1) */
            seq_t term2 = suffix[kk - 1];      /* sum_{j=kk-1}^{n-1} T(n-2,j) */
            row_cur[kk] = (seq_t)2 * term1 + term2;
        }

        /* Rotate rows for the next iteration. */
        seq_t *tmp    = row_n2;
        row_n2        = row_n1;
        row_n1        = row_cur;
        row_cur       = tmp;
    }

    seq_t result = row_n1[k];

    free(row_n2); free(row_n1); free(row_cur); free(suffix);
    return result;
}
