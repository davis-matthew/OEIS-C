/*
 * A354795  Triangle read by rows. The matrix inverse of A354794.
 *          Equivalently, the Bell transform of cfact(n) = -(n - 1)! if n > 0
 *          and otherwise 1/(-n)!.
 * https://oeis.org/A354795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the linear reading of the triangle T(n,k) defined by *
 *            T(0,0) = 1, * T(1,0) = 0, T(1,1) = 1, * for n ≥ 2 : T(n,0) = 0,
 *            T(n,n) = 1 * for 0 < k < n: * T(n,k) = (-1)^(n-k) *
 *            ((n-1)*T(n-2,k-1) - (n-1-k)*T(n-1,k) + T(n-1,k-1)) * a(n) is the
 *            (n)-th term of the triangle read row‑by‑row
 * Range:     i8 n <= 36, i16 n <= 55, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   _row_for_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/_row_for_index.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Determine row and column of the triangle */
    idx_t row = _row_for_index(n);
    __int128 tri = (__int128)row * (row + 1) / 2;
    idx_t offset = (idx_t)tri;
    idx_t col = n - offset;

    /* Base case: first element of the triangle */
    if (row == 0)
        return 1;

    /* Allocate and initialise row 0 and row 1 */
    seq_t *prevprev = (seq_t *)malloc(sizeof(seq_t));
    if (!prevprev) return 0;
    prevprev[0] = 1;

    seq_t *prev = (seq_t *)malloc(2 * sizeof(seq_t));
    if (!prev) { free(prevprev); return 0; }
    prev[0] = 0;
    prev[1] = 1;

    /* Handle the trivial second row */
    if (row == 1) {
        seq_t val = (col == 0) ? 0 : 1;
        if ((row - col) & 1)
            val = -val;
        free(prevprev);
        free(prev);
        return val;
    }

    /* Build rows up to the requested row using the recurrence for t(n,k) */
    for (idx_t r = 2; r <= row; ++r) {
        size_t sz = (size_t)r + 1;
        seq_t *curr = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!curr) {
            free(prevprev);
            free(prev);
            return 0;
        }
        curr[0] = 0;
        curr[r] = 1;
        for (idx_t k = 1; k < r; ++k) {
            seq_t term1 = (seq_t)(r - 1) * prevprev[k - 1];
            seq_t term2 = (seq_t)(r - 1 - k) * prev[k];
            curr[k] = term1 - term2 + prev[k - 1];
        }
        free(prevprev);
        prevprev = prev;
        prev = curr;
    }

    /* The desired value is in prev[col] with sign factor */
    seq_t result = prev[col];
    if (((row - col) & 1) != 0)
        result = -result;

    free(prevprev);
    free(prev);
    return result;
}
