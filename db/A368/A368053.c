/*
 * A368053  Array read by descending antidiagonals, where row n=0 lists the
 *          natural numbers. Each new row n=1,2,... is found by taking the
 *          number n in the previous row and "leaping" it over the next n
 *          terms to its right, while reversing the order of the skipped
 *          terms. (see example).
 * https://oeis.org/A368053
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

/* Sequence A368053: array read by descending antidiagonals.
   Each row r (r ≥ 1) is obtained from row r‑1 by moving the
   number r from the start of that row over the next r terms to
   the right, reversing the order of the skipped terms.
   The values are then read by collecting the entries along
   antidiagonals in order of increasing sum i + j, listing them
   from the lowest row index to the highest within each diagonal. */

#include <stdlib.h>
#include <stdint.h>

/* Build the rows up to index `up_to`.  row[0] holds the first
   element of row 0 (value 1).  All rows are stored in the static
   array `rows`.  Each row r has r+1 elements (indices 0 … r),
   elements with index > r are natural numbers and need not be
   stored. */
static seq_t **rows = NULL;
static size_t  rows_count = 0;   /* number of rows stored (next index to build) */
static size_t  rows_cap   = 0;   /* capacity of the rows array */

static void ensure_rows(idx_t up_to)
{
    if (rows_count > up_to)
        return;

    /* enlarge the rows pointer array if necessary */
    size_t need = up_to + 1;
    if (rows_cap < need) {
        size_t new_cap = rows_cap ? rows_cap : 1;
        while (new_cap < need)
            new_cap <<= 1;
        rows = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
        rows_cap = new_cap;
    }

    /* construct missing rows sequentially */
    for (idx_t r = rows_count; r <= up_to; ++r) {
        seq_t *row;
        if (r == 0) {
            /* row 0: only the first element (value 1) is needed. */
            row = (seq_t *)malloc(sizeof(seq_t));
            row[0] = (seq_t)1;
        } else {
            /* length of current row: r + 1 */
            size_t len = (size_t)(r + 1);
            row = (seq_t *)malloc(len * sizeof(seq_t));
            /* first element: the natural number r + 1 */
            row[0] = (seq_t)(r + 1);
            /* middle elements: reversed previous row's slice */
            for (idx_t j = 1; j < r; ++j) {
                /* previous row has length r, index r‑j is valid */
                row[j] = rows[r - 1][r - j];
            }
            /* last element: the number itself */
            row[r] = (seq_t)r;
        }
        rows[r] = row;
    }
    rows_count = up_to + 1;
}

/* Return a(n) for n ≥ 1.  a(0) is undefined, we return 0 in that case. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* out of range */

    /* find the diagonal index s (i + j = s) that contains a(n) */
    idx_t s = 0;
    /* use 128‑bit arithmetic to avoid overflow of the triangular number */
    while ((__uint128_t)(s + 1) * (s + 2) / 2 < n)
        ++s;

    idx_t prev = s * (s + 1) / 2;          /* number of elements before this diagonal */
    idx_t offset = n - prev - 1;           /* 0‑based offset within the diagonal */
    idx_t i = offset;                      /* row index (decreasing within a diagonal) */
    idx_t j = s - offset;                  /* column index */

    /* values to the right of the significant part are natural numbers */
    if (j > i)
        return (seq_t)(j + 1);

    /* otherwise the value is in the stored part of the row */
    ensure_rows(i);
    return rows[i][j];
}
