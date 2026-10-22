/*
 * A383753  Triangle T(n,k), n >= 0, 0 <= k <= n, read by rows, where T(n,k) =
 *          2^(n-k) * T(n-1,k-1) + 3^k * T(n-1,k) with T(n,k) = n^k if n*k=0.
 * https://oeis.org/A383753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = 2^(n-k) * T(n-1,k-1) + 3^k * T(n-1,k), T(n,0) = T(n,n)
 *            = 1, T(0,0) = 1. The sequence a(n) is the flattened triangle
 *            read by rows, a(0) = 1
 * Range:     u8 n <= 16, u16 n <= 23, u32 n <= 48, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   pow2, pow3, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"          /* pow2(idx_t exp)  */
#include "../utils/pow3.h"          /* pow3(idx_t exp)  */
#include "../utils/seq_alloc_new_array.h"  /* seq_alloc_new_array(size_t n) */

/* static storage for the already computed triangle rows */
static seq_t **row_ptr = NULL;          /* array of pointers to rows of T */
static idx_t   current_max_row = (idx_t)-1;  /* largest row that has been built */

/* build rows 0 … r (inclusive) */
static void ensure_rows(idx_t r)
{
    if (current_max_row != (idx_t)-1 && current_max_row >= r)
        return;             /* nothing to do */

    /* resize the array of row pointers */
    seq_t **tmp = (seq_t **)realloc(row_ptr,
                                    (size_t)(r + 1) * sizeof(seq_t *));
    if (!tmp)
        return;               /* out of memory – abort silently */
    row_ptr = tmp;

    /* compute new rows */
    idx_t start = (current_max_row == (idx_t)-1) ? 0 : (current_max_row + 1);
    for (idx_t n = start; n <= r; ++n) {
        seq_t *row = seq_alloc_new_array((size_t)(n + 1));
        if (!row)
            return;       /* out of memory – abort silently */
        row_ptr[n] = row;

        /* base values */
        row[0] = 1;          /* T(n,0) = 1 for all n */
        row[n] = 1;          /* T(n,n) = 1 for all n */

        /* interior elements via recurrence */
        for (idx_t k = 1; k < n; ++k) {
            seq_t left  = pow2(n - k);      /* 2^(n-k)          */
            left  = left * row_ptr[n - 1][k - 1];

            seq_t right = pow3(k);          /* 3^k              */
            right = right * row_ptr[n - 1][k];

            row[k] = left + right;
        }
    }
    current_max_row = r;
}

/* Return the n‑th term of the flattened triangle (0‑based). */
seq_t a(idx_t n)
{
    /* find the row r such that n lies in that row */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    const idx_t offset = r * (r + 1) / 2;
    const idx_t c = n - offset;              /* column index */

    ensure_rows(r);                          /* make sure row r exists */
    return row_ptr[r][c];
}
