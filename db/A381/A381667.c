/*
 * A381667  Triangle read by row: T(n,k) is the number of the k-th eliminated
 *          person in the variation of the Josephus elimination process for n
 *          people where two people are skipped each step.
 * https://oeis.org/A381667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: flatten the triangle T(n,k) of the Josephus variant
 * (skip two persons, eliminate the next one).  Each row n contains the
 * elimination order for n people.  The sequence a(n) is the row‑by‑row
 * read of that triangle.  For a given index n, we find the row r and
 * column c such that n = r(r−1)/2 + c, then simulate the elimination
 * for r people until the c‑th person is removed.
 */
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, n>=1 expected */

    /* Find row r containing the n-th element:
     * r*(r+1)/2 >= n > (r-1)*r/2
     */
    idx_t r = (idx_t)((sqrt((long double)8 * (long double)n + 1) - 1) / 2);

    /* Adjust in case of rounding errors */
    while ((r * (r + 1)) / 2 < n) ++r;
    while ((r > 1) && ((r - 1) * r / 2 >= n)) --r;

    /* Position within the row (1‑based) */
    idx_t col = n - (r - 1) * r / 2;

    /* Build an array of the remaining persons */
    size_t len = (size_t)r;
    seq_t *arr = (seq_t *)malloc(len * sizeof(seq_t));
    if (!arr) return 0;            /* allocation failure (should not happen) */
    for (size_t i = 0; i < len; ++i) arr[i] = (seq_t)(i + 1);

    size_t idx = 0;
    for (idx_t elim = 1; ; ++elim) {
        idx = (idx + 2) % len;          /* skip two people */
        seq_t removed = arr[idx];
        if (elim == col) {
            free(arr);
            return removed;
        }
        /* Remove the element by shifting the tail left */
        for (size_t j = idx; j + 1 < len; ++j) arr[j] = arr[j + 1];
        --len;
        if (len == 0) break;           /* safety, should not occur */
    }

    free(arr);
    return 0;                           /* unreachable */
}
