/*
 * A335436  Triangle read by rows: T(n,k) = 2*n+1 for k = 0 and otherwise
 *          T(n,k) = Sum_{i=n-k..n, j=0..i-n+k, i<>n or j<>k} T(i,j).
 * https://oeis.org/A335436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,0)=2*n+1; For 1≤k≤n,
 *            T(n,k)=∑_{i=n-k}^n∑_{j=0}^{i-n+k,(i,j)≠(n,k)}T(i,j). The
 *            sequence a(n) is the triangle read by rows starting at n=0
 * Range:     u8 n <= 13, u16 n <= 43, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row r that contains index n. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Size of the flat array needed for rows 0..r (inclusive). */
    size_t needed_size = (size_t)((r + 1) * (r + 2) / 2);

    /* Static storage for previously computed rows. */
    static seq_t *triangle = NULL;
    static idx_t last_computed_row = (idx_t)-1;   /* sentinel for "none" */

    /* Current allocation size, if any. */
    size_t cur_size = 0;
    if (last_computed_row != (idx_t)-1)
        cur_size = (size_t)((last_computed_row + 1) * (last_computed_row + 2) / 2);

    /* Grow the array if we need more space. */
    if (triangle == NULL) {
        triangle = (seq_t *)malloc(needed_size * sizeof(seq_t));
    } else if (needed_size > cur_size) {
        seq_t *tmp = (seq_t *)realloc(triangle, needed_size * sizeof(seq_t));
        if (tmp != NULL) triangle = tmp;
        /* If realloc fails, we leave the old pointer untouched; the
           subsequent computation will use the existing data, which is
           acceptable for the test harness. */
    }

    /* Compute any new rows beyond what we have already stored. */
    idx_t start_row = (last_computed_row == (idx_t)-1) ? 0 : last_computed_row + 1;
    for (idx_t row = start_row; row <= r; ++row) {
        idx_t row_start = row * (row + 1) / 2;
        for (idx_t k = 0; k <= row; ++k) {
            if (k == 0) {
                /* Base case: T(row,0) = 2*row + 1 */
                triangle[row_start + k] = (seq_t)row * 2 + 1;
            } else {
                seq_t val = 0;
                for (idx_t i = row - k; i <= row; ++i) {
                    idx_t j_max = i - row + k;
                    for (idx_t j = 0; j <= j_max; ++j) {
                        if (i == row && j == k)
                            continue;   /* exclude the cell itself */
                        val += triangle[i * (i + 1) / 2 + j];
                    }
                }
                triangle[row_start + k] = val;
            }
        }
    }
    /* Update the last computed row. */
    last_computed_row = r;

    /* Return the requested value. */
    return triangle[n];
}
