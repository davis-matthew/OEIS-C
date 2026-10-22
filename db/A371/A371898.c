/*
 * A371898  Triangle read by rows: T(n, k) = n * k * (T(n-1, k-1) + T(n-1, k))
 *          for k > 0 with initial values T(n, 0) = 1 and T(i, j) = 0 for j >
 *          i.
 * https://oeis.org/A371898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 40, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   T(n,k) = n * k * (T(n-1,k-1) + T(n-1,k)),   for k > 0
   T(n,0) = 1   for all n
   T(i,j) = 0   for j > i
   a(n) = T(r,k) where r(r+1)/2 ≤ n < (r+1)(r+2)/2
           and k = n – r(r+1)/2   (row‑major reading)
   This implementation builds the triangle lazily and memoises each row in a
   static array of rows.  It works for any seq_t type supported by the build
   (uint8_t … uint64_t, unsigned __int128, or an arbitrary‑precision class). */
#include <stdlib.h>            /* for realloc, abort */
#include "../utils/seq_alloc.h" /* seq_alloc(size_t) */

static seq_t **rows   = NULL;   /* rows[i] is a pointer to T(i,0..i) */
static idx_t   rows_alloc = 0;   /* number of rows currently allocated */

/* Ensure that all rows up to and including row 'r' are computed. */
static void ensure_row(idx_t r)
{
    if (r < rows_alloc)
        return;

    /* Grow the row pointer array to size r+1 */
    seq_t **tmp = (seq_t **)realloc(rows, sizeof(seq_t *) * (size_t)(r + 1));
    if (!tmp) abort();
    rows = tmp;

    /* Allocate and compute each new row */
    for (idx_t i = rows_alloc; i <= r; ++i) {
        /* allocate T(i,0..i) */
        seq_t *row = seq_alloc((size_t)(i + 1));
        if (!row) abort();
        rows[i] = row;

        /* base case */
        row[0] = 1;

        if (i == 0)
            continue;

        /* compute outer entries */
        seq_t *prev_row = rows[i - 1];
        for (idx_t k = 1; k <= i; ++k) {
            seq_t left  = prev_row[k - 1];
            seq_t right = (k <= i - 1) ? prev_row[k] : 0;
            seq_t sum   = left + right;          /* T(i-1,k-1)+T(i-1,k) */
            row[k] = (seq_t)i * (seq_t)k * sum;  /* n*k*sum */
        }
    }

    rows_alloc = r + 1;
}

/* Return the n‑th term of A371898 (row‑major triangle). */
seq_t a(idx_t n)
{
    /* Find r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t k = n - r * (r + 1) / 2;
    ensure_row(r);
    return rows[r][k];
}
