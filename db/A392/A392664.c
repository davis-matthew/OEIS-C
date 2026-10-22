/*
 * A392664  Triangle read by rows: T(n,k) (1 <= k <= n) given by T(n,1) =
 *          T(n,n) = 1, otherwise T(n,k) = (m*n-m*k+1)*T(n-1,k-1) +
 *          (m*k-m+1)*T(n-1,k), where m = -3.
 * https://oeis.org/A392664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the n‑th element when the triangle T(n,k) (with m = -3)
 *            defined by T(n,1)=T(n,n)=1, T(n,k) = (1-3*(n-k))*T(n-1,k-1) +
 *            (4-3*k)*T(n-1,k) (1<k<n) is read row by row. a(1)=1, a(2)=1,
 *            a(3)=1, a(4)=1, a(5)=-4, …
 * Range:     i8 n <= 17, i16 n <= 31, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row r for which the cumulative number of elements up to r
       is at least n.  Row r contains r elements, so the total up to row
       r is r*(r+1)/2. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* Position of the element within row r (1‑based). */
    idx_t pos = n - ((r - 1) * r) / 2;

    /* Handle the trivial first row directly. */
    if (r == 1)
        return (seq_t)1;

    /* Allocate the first row (row 1). */
    seq_t *prev = (seq_t *)malloc((size_t)1 * sizeof(seq_t));
    prev[0] = (seq_t)1;   /* T(1,1) */

    /* Build rows from 2 up to r. */
    for (idx_t row = 2; row <= r; ++row) {
        idx_t len = row;                            /* current row length */
        seq_t *curr = (seq_t *)malloc((size_t)len * sizeof(seq_t));

        curr[0] = (seq_t)1;                         /* first element T(row,1) */

        /* Compute interior elements.  k runs from 2 to row-1. */
        for (idx_t j = 1; j + 1 < len; ++j) {
            /* j is 0‑based index in curr; k = j+1 (1‑based column). */
            idx_t k = j + 1;
            seq_t term1 = ((seq_t)1 - (seq_t)3 * (seq_t)(len - k)) * prev[j - 1];
            seq_t term2 = ((seq_t)4 - (seq_t)3 * (seq_t)k) * prev[j];
            curr[j] = term1 + term2;
        }

        curr[len - 1] = (seq_t)1;                      /* last element T(row,row) */

        free(prev);                                    /* discard previous row */
        prev = curr;                                    /* current becomes previous */
    }

    /* Extract the desired element. */
    seq_t result = prev[pos - 1];                     /* pos is 1‑based */
    free(prev);                                      /* clean up */
    return result;
}
