/*
 * A285066  Triangle read by rows: T(n, m) = A285061(n, m)*m!, 0 <= m <= n.
 * https://oeis.org/A285066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r, m) where r = floor((sqrt(8*n+1)-1)/2) (the largest r
 *            with r*(r+1)/2 ≤ n) and m = n – r*(r+1)/2. The triangle T is
 *            defined by T(0,0) = 1 T(r,0) = 1 T(r,m) = 4*m*T(r-1,m-1) +
 *            (1+4*m)*T(r-1,m) for r ≥ 1, 0 ≤ m ≤ r
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 40, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* forward declarations of types provided by the build system */
/* (no typedefs here – they are defined elsewhere) */

/* static storage for the computed triangle rows */
static seq_t **_rows = NULL;      /* pointer to array of row pointers */
static idx_t  _rows_allocated = 0; /* number of rows already computed */

/* --------------------------------------------------------------------
   Ensure that all rows up to 'target' (inclusive) are computed and
   stored in _rows.  Rows are created incrementally as needed.
   -------------------------------------------------------------------- */
static void _compute_rows_up_to(idx_t target)
{
    if (_rows_allocated > target)
        return;   /* nothing to do */

    /* Allocate or grow the array of row pointers if necessary. */
    seq_t **new_rows = (seq_t **)realloc(_rows,
                                         sizeof(seq_t *) * (target + 1));
    _rows = new_rows;

    /* Build any missing rows. */
    for (idx_t r = _rows_allocated; r <= target; ++r)
    {
        /* Allocate memory for row r (indices 0..r). */
        seq_t *row = (seq_t *)malloc(sizeof(seq_t) * (r + 1));
        _rows[r] = row;

        /* Base case: first row. */
        if (r == 0)
        {
            row[0] = (seq_t)1;
            continue;
        }

        /* T(r,0) = 1. */
        row[0] = (seq_t)1;

        /* Remaining entries. */
        seq_t *prev_row = _rows[r - 1];
        for (idx_t m = 1; m <= r; ++m)
        {
            /* term1 = 4 * m * T(r-1, m-1) */
            seq_t term1 = ((seq_t)4 * (seq_t)m) * prev_row[m - 1];

            /* term2 = (1 + 4*m) * T(r-1, m)  (zero if m == r) */
            seq_t term2 = 0;
            if (m < r)
                term2 = ((seq_t)1 + (seq_t)4 * (seq_t)m) * prev_row[m];

            row[m] = term1 + term2;
        }
    }

    _rows_allocated = target + 1;
}

/* --------------------------------------------------------------------
   Main function: compute a(n).  It decomposes n into r and m as
   described above, ensures all needed rows are present, and returns
   the triangle entry T(r, m).
   -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Find r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2.  A simple
       linear search is adequate because r grows only as O(√n). */
    idx_t r = 0;
    idx_t t = 0;           /* triangular number r*(r+1)/2 */
    while (t <= n)
    {
        ++r;
        t = (idx_t)r * (r + 1) / 2;
    }
    --r;                   /* r is now the largest suitable value */
    t = (idx_t)r * (r + 1) / 2;
    idx_t m = n - t;       /* column index within row r */

    /* Ensure the triangle rows up to r are computed. */
    _compute_rows_up_to(r);

    /* Return the requested entry. */
    return _rows[r][m];
}
