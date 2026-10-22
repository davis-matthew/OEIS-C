/*
 * A395459  Triangle read by rows, T(1,0)=1, T(n,m) = (-1)^(n+m-1) * (n-2)! *
 *          (n+m-1) / m! for n >= 2, 0 <= m <= n-1.
 * https://oeis.org/A395459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) is the n‑th entry of the triangle T read by rows. T(1,0)=1;
 *            for n>=2, 0<=m<=n-1, T(n,m)=(-1)^(n+m-1)*(n-2)!*(n+m-1)/m!
 *            (integer for m<=n-1). The sequence a(n) is obtained by
 *            flattening the rows of T
 * Range:     i8 n <= 16, i16 n <= 36, i32 n <= 91, i64 n <= 231,
 *            i128 n <= 595, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
#  define ALLOC_ARRAY(type,n)  new type[(n)]
#else
#  define ALLOC_ARRAY(type,n)  ((type*)malloc(sizeof(type)*(n)))
#endif

seq_t a(idx_t n)
{
    /* The very first term */
    if (n == 1) return (seq_t)1;

    /* Determine the row r containing position n:
       r*(r-1)/2 < n <= r*(r+1)/2  (1‑based rows)      */
    idx_t r = 1;
    while (n > (idx_t)(r * (r + 1) / 2)) ++r;

    /* Static storage for the triangle and factorial of (k-2)! for the
       current highest computed row. */
    static seq_t **tbl = NULL;   /* tbl[k] points to an array of k elements */
    static idx_t tbl_rows = 0;   /* highest row that has been computed */
    static seq_t fact_prev = 1;  /* holds (k-2)! when row k is about to be computed */

    /* Allocate the row pointers array if necessary */
    if (tbl_rows < r) {
        idx_t new_rows = r + 1;   /* we want indices 0..r inclusive */
        if (tbl == NULL) {
            tbl = ALLOC_ARRAY(seq_t*, new_rows);
        } else {
            seq_t **tmp = ALLOC_ARRAY(seq_t*, new_rows);
            /* copy old pointers */
            for (idx_t i = 0; i <= tbl_rows; ++i) tmp[i] = tbl[i];
            /* free old pointer array */
#ifdef __cplusplus
            delete[] tbl;
#else
            free(tbl);
#endif
            tbl = tmp;
        }
        if (tbl == NULL) return (seq_t)0;   /* out of memory */

        /* Compute missing rows */
        for (idx_t k = tbl_rows + 1; k <= r; ++k) {
            tbl[k] = ALLOC_ARRAY(seq_t, k);
            if (tbl[k] == NULL) return (seq_t)0;   /* out of memory */

            if (k == 1) {                 /* row 1 */
                tbl[1][0] = (seq_t)1;
                fact_prev = (seq_t)1; /* (1-2)! = (-1)! not used, keep 1 */
                continue;
            }

            /* Entry at m=0: (-1)^(k-1)*(k-1)*(k-2)! */
            seq_t val0 = (seq_t)(k - 1) * fact_prev;
            if ((k - 1) & 1) val0 = -val0;
            tbl[k][0] = val0;

            /* Right edge: m = k-1 */
            tbl[k][k - 1] = (seq_t)2;

            /* Interior entries via recurrence */
            for (idx_t m = 1; m <= k - 2; ++m) {
                seq_t left  = tbl[k - 1][m - 1];
                seq_t right = tbl[k - 1][m];
                seq_t mult  = (seq_t)(k - 1 - m) * right;
                tbl[k][m] = left - mult;
            }

            /* Update fact_prev to (k-1)! for next iteration */
            fact_prev = fact_prev * (seq_t)(k - 1);
        }
        tbl_rows = r;
    }

    /* Locate the column index m within row r */
    idx_t prevTri = (r - 1) * r / 2;           /* number of terms before row r */
    idx_t m       = n - prevTri - 1;          /* 0‑based column index */

    return tbl[r][m];
}
