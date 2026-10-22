/*
 * A341695  Regular triangle read by rows, T(n,k) =
 *          T(n,k-1)+2*T(n-1,k)-T(n-1,k-1) for 1<=k<=n-2 with
 *          T(n,n)=T(n,n-1)=T(n,n-2) for n>=3 and T(1,1)=T(2,1)=T(2,2)=1.
 * https://oeis.org/A341695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the triangle: T(1,1)=T(2,1)=T(2,2)=1;
   for n>=3:  T(n,1)=2*T(n-1,1);
   for 2<=k<=n-2:   T(n,k)=T(n,k-1)+2*T(n-1,k)-T(n-1,k-1);
   for n>=3:         T(n,n)=T(n,n-1)=T(n,n-2);
   a(n) is the triangle read by rows. */

#include <stddef.h>
#include <stdlib.h>

/* static storage for computed rows of the triangle */
static seq_t **T_rows = NULL;      /* array of pointers to rows (1‑based) */
static size_t T_rows_len = 0;     /* highest row index computed so far */
static size_t T_rows_cap = 0;     /* capacity of the T_rows array */

/* ensure that rows up to 'up_to' are computed and stored */
static void ensure_rows(size_t up_to) {
    if (T_rows_len >= up_to) return;

    if (T_rows_cap < up_to + 1) {
        seq_t **tmp = (seq_t **)realloc(T_rows, sizeof(seq_t *) * (up_to + 1));
        if (!tmp) exit(EXIT_FAILURE);
        T_rows = tmp;
        T_rows_cap = up_to + 1;
    }

    for (size_t r = T_rows_len + 1; r <= up_to; ++r) {
        seq_t *row = (seq_t *)malloc(sizeof(seq_t) * (r + 1));
        if (!row) exit(EXIT_FAILURE);

        if (r == 1) {
            row[1] = (seq_t)1;
        } else if (r == 2) {
            row[1] = (seq_t)1;
            row[2] = (seq_t)1;
        } else {
            seq_t *prev = T_rows[r - 1];   /* previous row */

            /* first element */
            row[1] = prev[1] + prev[1];

            /* elements 2 .. r-2 using the recurrence */
            for (size_t k = 2; k <= r - 2; ++k) {
                seq_t temp = prev[k] + prev[k];       /* 2*prev[k] */
                row[k] = row[k - 1] + temp - prev[k - 1];
            }

            /* the last three elements equal the (r-2)-th element */
            row[r - 1] = row[r - 2];
            row[r]     = row[r - 2];
        }

        T_rows[r] = row;
    }

    T_rows_len = up_to;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;

    /* find the row number r such that r*(r+1)/2 >= n */
    idx_t triangular = 0;
    size_t r = 0;
    while (triangular < n) {
        ++r;
        triangular += r;
    }

    size_t prev_triangular = (size_t)(triangular - r);   /* sum of 1..r-1 */
    size_t k = (size_t)(n - prev_triangular);          /* 1‑based column */

    ensure_rows(r);
    return T_rows[r][k];
}
