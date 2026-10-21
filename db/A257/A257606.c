/*
 * A257606  Triangle read by rows: T(n,k) = t(n-k, k); t(n,m) = f(m)*t(n-1,m)
 *          + f(n)*t(n,m-1), where f(x) = x + 4.
 * https://oeis.org/A257606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8n+1)-1)/2), k = n -
 *            r(r+1)/2. Triangle T is defined by: T(0,0) = 1, T(n,0) = T(n,n)
 *            = 4^n, for 0<k<n: T(n,k) = (k+4)·T(n-1,k) + (n-k+4)·T(n-1,k-1).
 *            The function caches each row of the triangle in a static array
 *            of pointers. All arithmetic is performed in type...
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 57, u64 n <= 159, u128 n <= 478,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* -------------------------------------------------------------------------- */
/* Integer square‑root: floor(sqrt(x)) for unsigned 64‑bit integers.           */
/* -------------------------------------------------------------------------- */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t bit = ((idx_t)1u << 32);          /* largest power of two ≤ sqrt(UINT64_MAX) */

    while (bit) {
        idx_t nxt = r + bit;
        /* avoid overflow: nxt <= x / nxt  ⇔  nxt^2 <= x */
        if (nxt != 0 && nxt <= x / nxt) {
            r = nxt;
        }
        bit >>= 1;
    }
    return r;
}

/* -------------------------------------------------------------------------- */
/* Static storage for the triangle rows (each row is a dynamic array).         */
/* -------------------------------------------------------------------------- */
static seq_t **rows = NULL;        /* pointers to rows 0,1,… */
static idx_t  rows_cap = 0;        /* capacity of the above array      */
static idx_t  max_row_computed = 0;/* highest row that has been built   */
static seq_t  pow4_cur = (seq_t)1; /* 4^max_row_computed, ready for next row */

/* -------------------------------------------------------------------------- */
/* Ensure that all rows up to index 'r' (inclusive) have been generated.       */
/* -------------------------------------------------------------------------- */
static void ensure_rows_up_to(idx_t r)
{
    if (rows == NULL) { /* first call – initialise */
        rows_cap = 1;
        rows = (seq_t **)malloc(rows_cap * sizeof(seq_t *));
        rows[0] = (seq_t *)malloc(sizeof(seq_t));
        rows[0][0] = pow4_cur; /* 4^0 = 1 */
        max_row_computed = 0;
        pow4_cur = pow4_cur * (seq_t)4; /* now holds 4^1 for the next row */
    }

    while (max_row_computed < r) {
        idx_t new_i = max_row_computed + 1;   /* index of the row to build */

        /* Grow the array of row pointers if necessary */
        if (rows_cap <= new_i) {
            rows_cap = new_i + 1;
            rows = (seq_t **)realloc(rows, rows_cap * sizeof(seq_t *));
        }

        /* Allocate the new row */
        rows[new_i] = (seq_t *)malloc((new_i + 1) * sizeof(seq_t));

        seq_t *prev = rows[new_i - 1];
        seq_t *curr = rows[new_i];
        seq_t pow4  = pow4_cur;                /* 4^new_i, left boundary */

        curr[0] = pow4;                        /* T(new_i,0) = 4^new_i */

        /* Interior elements */
        for (idx_t k = 1; k < new_i; ++k) {
            seq_t term1 = (seq_t)(k + 4) * prev[k];
            seq_t term2 = (seq_t)(new_i - k + 4) * prev[k - 1];
            curr[k] = term1 + term2;           /* recurrence */
        }
        curr[new_i] = pow4;                    /* T(new_i,new_i) = 4^new_i */

        /* Prepare pow4_cur for the next iteration */
        pow4_cur = pow4_cur * (seq_t)4;
        ++max_row_computed;
    }
}

/* -------------------------------------------------------------------------- */
/* Public function returning the n-th term of the sequence.                   */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Determine the row r and column k that contain a(n).  The triangle
     * is read row by row, so the total number of elements before row r is
     * r*(r+1)/2.  We solve (sqrt(8n+1)-1)/2 for r. */
    idx_t m = n;
    idx_t sqrt_term = isqrt_u64(8 * m + 1);  /* floor(sqrt(8n+1)) */
    idx_t r = (sqrt_term - 1) / 2;           /* row index (0‑based) */
    idx_t k = m - r * (r + 1) / 2;           /* column index within row */

    /* Build rows up to r if necessary and return the requested element. */
    ensure_rows_up_to(r);
    return rows[r][k];
}
