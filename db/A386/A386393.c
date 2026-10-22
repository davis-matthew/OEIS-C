/*
 * A386393  Triangle T(n, k) (1 <= k <= n) read by rows: T(n, k) is the
 *          numerator of R(n, k) where R(n, k) = R(n, k-1)/2 + R(n-1, k-1) for
 *          1 < k <= n with R(n, 1) = R(n-1, n-1) for n > 1, R(1, 1) = 1.
 * https://oeis.org/A386393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, frac
 * Formula:   * Let T(r,k) be the triangle defined by * T(1,1) = 1; * T(r,1) =
 *            T(r-1,r-1) (r > 1); * T(r,k) = T(r,k-1) + T(r-1,k-1) * 2^(r-1)
 *            (2 ≤ k ≤ r). * The sequence a(n) is obtained by reading this
 *            triangle row by row, * i.e. a(n) = T(row, position_in_row) with
 *            1‑based indexing
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 34, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* undefined for offset 0, keep safe */

    /* Find the row r such that r*(r+1)/2 ≥ n */
    idx_t r = 1;
    idx_t cum = 1;
    while (cum < n) {
        ++r;
        cum += r;
    }

    /* Position k within this row (1‑based) */
    idx_t k = n - (r - 1) * r / 2;

    /* Handle the trivial first row */
    if (r == 1) return (seq_t)1;

    /* Allocate storage for the previous row (row 1) */
    size_t prev_len = 2;                    /* indices 0..1 */
    seq_t *prev = (seq_t *)malloc(prev_len * sizeof(seq_t));
    prev[1] = (seq_t)1;                     /* T(1,1) = 1 */

    seq_t pow2 = (seq_t)1;                  /* 2^(row-1) for the current row */

    /* Build rows 2 .. r */
    for (idx_t row = 2; row <= r; ++row) {
        pow2 <<= 1;                         /* 2^(row-1) */
        size_t curr_len = (size_t)(row + 1);/* indices 0..row */
        seq_t *curr = (seq_t *)malloc(curr_len * sizeof(seq_t));

        curr[1] = prev[row - 1];            /* T(row,1) = T(row-1,row-1) */

        for (idx_t j = 2; j <= row; ++j) {
            seq_t term = prev[j - 1];
            term *= pow2;                   /* T(row-1,j-1) * 2^(row-1) */
            curr[j] = curr[j - 1] + term;   /* T(row,j) = prev + term */
        }

        free(prev);
        prev = curr;
        prev_len = curr_len;
    }

    /* Extract the required element */
    seq_t result = prev[k];
    free(prev);
    return result;
}
