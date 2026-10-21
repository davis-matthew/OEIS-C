/*
 * A268434  Triangle read by rows, Lah numbers of order 2, T(n,n) = 1, T(n,k)
 *          = 0 if k<0 or k>n, otherwise T(n,k) =
 *          T(n-1,k-1)+((n-1)^2+k^2)*T(n-1,k), for n>=0 and 0<=k<=n.
 * https://oeis.org/A268434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n+1)th element when the triangle T(n,k) is read by
 *            rows, where T(n,k) satisfies: T(0,0) = 1 T(n,k) = 0 if k < 0 or
 *            k > n T(n,k) = T(n-1,k-1) + ((n-1)^2 + k^2) * T(n-1,k) for 0 ≤ k
 *            ≤ n The function builds the triangle incrementally, caching all
 *            rows that have been computed
 * Range:     u8 n <= 15, u16 n <= 22, u32 n <= 45, u64 n <= 105,
 *            u128 n <= 253, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static state for incremental computation */
    static seq_t **rows      = NULL;   /* array of pointers to each row */
    static size_t  rows_cap  = 0;      /* capacity of rows array       */
    static size_t  rows_len  = 0;      /* number of rows stored        */
    static size_t  total_elems = 0;    /* total number of elements in rows */

    /* Helper to grow the rows array */
    if (rows_len + 1 > rows_cap) {
        size_t new_cap = rows_cap ? rows_cap * 2 : 8;
        seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
        rows = tmp;
        rows_cap = new_cap;
    }

    /* Create row 0 if needed */
    if (rows_len == 0) {
        seq_t *row0 = (seq_t *)malloc(sizeof(seq_t));
        if (!row0) return 0;        /* OOM guard – will abort on real run */
        row0[0] = (seq_t)1;
        rows[0] = row0;
        rows_len = 1;
        total_elems = 1;
    }

    /* Build rows until we have enough elements to cover index n */
    while (total_elems <= (size_t)n) {
        size_t next_row   = rows_len;          /* index of new row            */
        size_t next_len   = next_row + 1;      /* length of new row           */
        seq_t  *newrow    = (seq_t *)malloc(sizeof(seq_t) * next_len);
        if (!newrow) return 0;                 /* OOM guard */
        seq_t  *prev_row  = rows[rows_len - 1];
        size_t prev_len   = next_row;           /* prev_row length equals next_row */

        /* Base factor (next_row-1)^2 */
        seq_t base = (seq_t)(next_row - 1) * (seq_t)(next_row - 1);

        /* k = 0 */
        newrow[0] = base * prev_row[0];

        /* k ≥ 1 */
        for (size_t k = 1; k < next_len; ++k) {
            seq_t prev_k_minus1 = prev_row[k - 1];
            seq_t prev_k        = (k < prev_len) ? prev_row[k] : 0;
            seq_t coeff         = base + (seq_t)k * (seq_t)k;
            newrow[k] = prev_k_minus1 + coeff * prev_k;
        }

        /* Store the new row */
        rows[rows_len] = newrow;
        rows_len++;
        total_elems += next_len;

        /* Grow the rows array if necessary (already pre‑checked above) */
        if (rows_len > rows_cap) {
            size_t new_cap = rows_cap ? rows_cap * 2 : 8;
            seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
            rows = tmp;
            rows_cap = new_cap;
        }
    }

    /* Locate the row and position that contain index n */
    size_t r          = rows_len - 1;                     /* current last row index */
    size_t prev_total = total_elems - rows_len;           /* elements before row r */
    size_t k          = (size_t)n - prev_total;           /* column within row r */

    return rows[r][k];
}
