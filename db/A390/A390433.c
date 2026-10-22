/*
 * A390433  Triangle read by rows, Lah numbers with level 2.
 * https://oeis.org/A390433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 37, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A390433: Triangle read by rows, Lah numbers with level 2
 *  Recurrence:   T(n,k) = T(n-1,k-1) + (n+k-1)^2 · T(n-1,k)
 *  Boundary:     T(0,0)=1; T(n,0)=0 for n>0; T(n,n)=1
 *  a(n) is the n‑th element of the triangle read row‑major.
 *  The function accepts any idx_t ≥0 and returns the corresponding
 *  seq_t value.  It caches computed rows for repeated calls.
 */

#include <stdlib.h>

/* external types are supplied by the build system:
 *   - seq_t : unsigned integer type (uint8_t … uint128 or big integer)
 *   - idx_t : unsigned 64‑bit integer type
 */

/* ------------------------------------------------------------------ */
/*  Indexed access to the triangle                                      */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* static cache of computed rows                                  */
    static seq_t **triangle = NULL;   /* array of pointers to rows   */
    static idx_t rows     = 0;        /* number of rows allocated (next unused index) */

    /* Determine the row r and column c that contain the n‑th element.  */
    idx_t r         = 0;              /* candidate row               */
    idx_t accumulated = 0;           /* total elements before row r */

    /* Find r such that accumulated + (r+1) > n  (i.e. the element lies in row r) */
    while (accumulated + (r + 1) <= n) {
        accumulated += r + 1;         /* count elements of row r   */
        ++r;                         /* move to next row          */
    }
    /* The n‑th element is in row r, column c = n - accumulated */
    idx_t c = n - accumulated;

    /* Ensure rows up to r are computed */
    while (rows <= r) {
        idx_t idx = rows;               /* index of new row          */
        seq_t *row = (seq_t *)malloc((idx + 1) * sizeof(seq_t));
        if (!row) abort();              /* memory failure            */

        if (idx == 0) {                 /* first row T(0,0) = 1       */
            row[0] = (seq_t)1;

            /* set up the pointer array for the first row  */
            triangle = (seq_t **)malloc(1 * sizeof(seq_t *));
            if (!triangle) abort();
            triangle[0] = row;
            ++rows;
            continue;
        }

        /* boundary values for n>0                                 */
        row[0] = (seq_t)0;             /* T(idx,0) = 0           */
        row[idx] = (seq_t)1;           /* T(idx,idx) = 1         */

        /* compute interior entries using the recurrence          */
        for (idx_t k = 1; k < idx; ++k) {
            seq_t left   = triangle[idx-1][k-1];    /* T(idx-1,k-1) */
            seq_t above  = triangle[idx-1][k];      /* T(idx-1,k) */
            seq_t mult   = (seq_t)(idx + k - 1);    /* (n+k-1)     */
            mult = mult * mult;                     /* square      */
            row[k] = left + above * mult;           /* recurrence  */
        }

        /* Expand the pointer array to hold the new row           */
        seq_t **tmp = (seq_t **)realloc(triangle,
                                        (idx + 1) * sizeof(seq_t *));
        if (!tmp) abort();
        triangle = tmp;
        triangle[idx] = row;
        ++rows;
    }

    /* Return the requested element                               */
    return triangle[r][c];
}
