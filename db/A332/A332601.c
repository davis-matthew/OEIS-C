/*
 * A332601  An RNA-type Riordan lower-triangular matrix, read by rows.
 * https://oeis.org/A332601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 119, u32 n <= 378, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A332601 – RNA‑type Riordan lower‑triangular matrix, read by rows
 *
 *  The triangle T(n,k) is defined for n,k ≥ 0, 0 ≤ k ≤ n by
 *
 *      T(0,0)           = 1
 *      T(1,0) = T(1,1)   = 1           (start of first non‑trivial row)
 *      T(n,n)           = 1           (the last entry of every row)
 *      T(n,k) = T(n-1,k+1)            +  Σ_{j=0..k} T(n-1-j,k-j)
 *                       (for 0 ≤ k < n)
 *
 *  The sequence a(n) is obtained by reading the triangle row by row
 *  (row 0 first, then row 1, …).  That is,
 *
 *      a(n) = T(r,c)  where  r = smallest integer with
 *              (r+1)(r+2)/2 > n
 *              and  c = n - r(r+1)/2 .
 *
 *  The following implementation builds rows on demand and caches them
 *  in static storage.  It works for all integer types supplied for seq_t
 *  and idx_t, including arbitrary‑precision seq_t from a C++ build.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of triangle rows (tri[i][j] = T(i,j)) */
    static seq_t **tri = NULL;
    static idx_t rows  = 0;       /* number of rows currently cached */

    /* find the row r that contains index n */
    idx_t r = 0;
    /* use 128‑bit intermediate to avoid overflow for very large n */
    while ( (unsigned __int128)(r + 1) * (r + 2) / 2 <= n )
        ++r;

    /* extend the cache up to row r if necessary */
    if (r >= rows) {
        tri = (seq_t **)realloc(tri, (size_t)(r + 1) * sizeof(seq_t *));
        for (idx_t i = rows; i <= r; ++i) {
            tri[i] = (seq_t *)malloc((size_t)(i + 1) * sizeof(seq_t));
            if (i == 0) {
                tri[i][0] = (seq_t)1;
            } else if (i == 1) {
                tri[i][0] = (seq_t)1;
                tri[i][1] = (seq_t)1;
            } else {
                /* compute interior entries */
                for (idx_t k = 0; k < i; ++k) {
                    seq_t val = 0;
                    if (k < i - 1)
                        val += tri[i - 1][k + 1];          /* T(n‑1,k+1) */

                    /* Σ_{j=0..k} T(n-1-j,k-j) */
                    for (idx_t j = 0; j <= k; ++j)
                        val += tri[i - 1 - j][k - j];

                    tri[i][k] = val;
                }
                /* last entry of the row */
                tri[i][i] = (seq_t)1;
            }
        }
        rows = r + 1;
    }

    idx_t offset = r * (r + 1) / 2;   /* number of terms before row r */
    idx_t col    = n - offset;        /* column within the row   */

    return tri[r][col];
}
