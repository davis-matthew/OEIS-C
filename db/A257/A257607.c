/*
 * A257607  Triangle read by rows: T(n,k) = t(n-k, k); t(n,m) = f(m)*t(n-1,m)
 *          + f(n)*t(n,m-1), where f(x) = x + 5.
 * https://oeis.org/A257607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattened T‑triangle with T(n,0)=5ⁿ and for 1≤k≤n-1:
 *            T(n,k) = (k+5)*T(n-1,k) + (n-k+5)*T(n-1,k-1), T(n,n)=T(n,0).
 *            a(n) = T(row,col) where row and col are found from the
 *            triangular indexing. Initial values: a(0)=1, a(1)=5, a(2)=5,
 *            a(3)=25, …
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 48, u64 n <= 158, u128 n <= 475,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static storage for computed rows */
static seq_t **rows = NULL;            /* rows[i] points to array of T(i,0..i) */
static size_t rows_count = 0;          /* number of rows currently stored */
static seq_t pow5_value = 1;           /* 5^i for the last computed row i */

static void ensure_rows(idx_t r)
{
    if (rows == NULL) {
        rows = NULL;
        rows_count = 0;
    }
    if ((size_t)r < rows_count)
        return;   /* already have up to row r */

    /* Grow array of row pointers if needed */
    if (rows_count == 0) {
        rows = (seq_t **)malloc((r + 1) * sizeof(seq_t *));
    } else if ((size_t)r + 1 > rows_count) {
        rows = (seq_t **)realloc(rows, ((size_t)r + 1) * sizeof(seq_t *));
    }

    /* Compute new rows from rows_count up to r */
    for (idx_t i = rows_count; i <= r; ++i) {
        size_t len = (size_t)(i + 1);
        seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));

        if (i == 0) {                /* T(0,0) = 1, 5^0 = 1 */
            row[0] = 1;
            pow5_value = 1;
        } else {
            pow5_value = pow5_value * (seq_t)5;  /* 5^i */
            row[0] = pow5_value;                /* T(i,0)   */
            row[i] = pow5_value;                /* T(i,i)   */
            /* Compute middle entries */
            for (idx_t k = 1; k < i; ++k) {
                seq_t a = rows[i - 1][k] * (seq_t)(k + 5);
                seq_t b = rows[i - 1][k - 1] * (seq_t)(i - k + 5);
                row[k] = a + b;
            }
        }
        rows[i] = row;
    }
    rows_count = (size_t)(r + 1);
}

/* Locate row and column for flattened index n (0‑based) */
static void find_row_col(idx_t n, idx_t *row, idx_t *col)
{
    idx_t r = 0;
    /* Find r such that (r+1)(r+2)/2 > n >= r(r+1)/2 */
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    *row = r;
    *col = n - r * (r + 1) / 2;   /* elements before row r */
}

seq_t a(idx_t n)
{
    idx_t r, k;
    find_row_col(n, &r, &k);
    ensure_rows(r);
    return rows[r][k];
}
