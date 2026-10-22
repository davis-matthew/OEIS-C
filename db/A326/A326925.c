/*
 * A326925  Irregular triangular array read by rows: row n shows the
 *          coefficients of this polynomial of degree n: (1/n!)*(numerator of
 *          n-th derivative of (1-x)/(1-x-x^2)).
 * https://oeis.org/A326925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, sign
 * Formula:   not stated
 * Range:     i8 n <= 46, i16 n <= 123, i32 n <= 384, i64 n <= 1290,
 *            i128 n <= 4690, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A326925
    Irregular triangular array read by rows.
    The array of coefficients a[r][k] satisfies the recurrence
        a_{r,k} = a_{r-1,k} + 2 a_{r-1,k-1}
                  - a_{r-2,k-2} - a_{r-2,k-1} + a_{r-2,k}
    with initial rows
        a_0 = [1, -1]
        a_1 = [0, 2, -1]
    The k‑th entry in row r is returned as a(n) where the rows are
    flattened in order of increasing r.
    In particular a(1)=1, a(2)=-1, a(3)=0, a(4)=2, ….
*/

#include <stdlib.h>

/* Global cache for the rows of the array. */
static seq_t **rows = NULL;          /* pointers to rows a_0, a_1, …   */
static idx_t rows_count = 0;         /* number of rows currently stored */

/* Create/extend the cache so that rows[0..r] exist.  */
static void ensure_rows(idx_t r)
{
    if (r < rows_count)
        return;

    size_t new_cap = (size_t)(r + 1);
    seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
    if (!tmp) /* allocation failed – terminate program */
        abort();
    rows = tmp;

    for (idx_t i = rows_count; i <= r; ++i) {
        size_t sz = (size_t)(i + 2);                    /* length of row i */
        seq_t *arr = (seq_t *)calloc(sz, sizeof(seq_t));
        if (!arr)
            abort();

        if (i == 0) {                                  /* a_0 = [1, -1]      */
            arr[0] = (seq_t)1;
            arr[1] = (seq_t)(-1);
        } else if (i == 1) {                           /* a_1 = [0, 2, -1]   */
            arr[0] = (seq_t)0;
            arr[1] = (seq_t)2;
            arr[2] = (seq_t)(-1);
        } else {                                       /* recurrence for i>=2 */
            seq_t *prev     = rows[i - 1];
            seq_t *prevprev = rows[i - 2];
            size_t prev_sz     = (size_t)(i + 1); /* row (i-1) has i+1 elements   */
            size_t prevprev_sz = (size_t)(i);     /* row (i-2) has i   elements   */
            for (size_t k = 0; k < sz; ++k) {
                seq_t val = (seq_t)0;

                if (k < prev_sz)                       /* a_{i-1,k}          */
                    val += prev[k];
                if (k >= 1 && k - 1 < prev_sz)         /* 2*a_{i-1,k-1}      */
                    val += (seq_t)2 * prev[k - 1];
                if (k < prevprev_sz)                   /* a_{i-2,k}          */
                    val += prevprev[k];
                if (k >= 2 && k - 2 < prevprev_sz)     /* -a_{i-2,k-2}       */
                    val -= prevprev[k - 2];
                if (k >= 1 && k - 1 < prevprev_sz)     /* -a_{i-2,k-1}       */
                    val -= prevprev[k - 1];

                arr[k] = val;
            }
        }
        rows[i] = arr;
    }

    rows_count = r + 1;
}

/*  Return the n-th term of A326925  (n ≥ 1).  */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    /* Find the row index r (0‑based) such that the first r rows contain
       less than n terms, while r+1 rows contain at least n terms.
       The number of terms through row r is (r+1)(r+4)/2.  */
    idx_t r = 0;
    while (1) {
        idx_t terms_through_row = ((r + 1) * (r + 4)) / 2;
        if (n <= terms_through_row)
            break;
        ++r;
    }

    ensure_rows(r);

    /* Number of terms before row r */
    idx_t terms_before = (r == 0) ? 0 : (r * (r + 3)) / 2;
    idx_t k = n - terms_before - 1;      /* 0‑based coefficient index */

    return rows[r][k];
}
