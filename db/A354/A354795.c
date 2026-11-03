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
 * Validated: partial, pass, 66 terms through n = 65 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
static inline idx_t _row_for_index(idx_t n)
{
    idx_t r = 0;
    while (1) {
        __int128 limit = (__int128)(r + 1) * (r + 2) / 2;
        if (limit > n)
            break;
        ++r;
    }
    return r;
}

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
    seq_t *prevprev = mem_new(seq_t, 1);
    if (!prevprev) return 0;
    prevprev[0] = 1;

    seq_t *prev = mem_new(seq_t, 2);
    if (!prev) { mem_free(prevprev); return 0; }
    prev[0] = 0;
    prev[1] = 1;

    /* Handle the trivial second row */
    if (row == 1) {
        seq_t val = (col == 0) ? 0 : 1;
        if ((row - col) & 1)
            val = -val;
        mem_free(prevprev);
        mem_free(prev);
        return val;
    }

    /* Build rows up to the requested row using the recurrence for t(n,k) */
    for (idx_t r = 2; r <= row; ++r) {
        size_t sz = (size_t)r + 1;
        seq_t *curr = mem_new(seq_t, sz);
        if (!curr) {
            mem_free(prevprev);
            mem_free(prev);
            return 0;
        }
        curr[0] = 0;
        curr[r] = 1;
        for (idx_t k = 1; k < r; ++k) {
            seq_t term1 = (seq_t)(r - 1) * prevprev[k - 1];
            seq_t term2 = (seq_t)(r - 1 - k) * prev[k];
            curr[k] = term1 - term2 + prev[k - 1];
        }
        mem_free(prevprev);
        prevprev = prev;
        prev = curr;
    }

    /* The desired value is in prev[col] with sign factor */
    seq_t result = prev[col];
    if (((row - col) & 1) != 0)
        result = -result;

    mem_free(prevprev);
    mem_free(prev);
    return result;
}
