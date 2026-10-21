/*
 * A279009  Alternating Jacobsthal triangle A_{-2}(n,k) read by rows.
 * https://oeis.org/A279009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 65, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: Alternating Jacobsthal triangle A_{-2}(n,k) read by rows.
 * A(n,0) = (-2)^{⌊n/2⌋},  A(n,n) = 1.
 * For 0 < k < n   A(n,k) = A(n-1,k-1) – A(n-1,k).
 * The sequence a(n) is obtained by flattening the rows left‑to‑right.
 */
#include <stdlib.h>

/* external types are supplied by the build system */
seq_t a(idx_t n)
{
    /* Flattened triangle stored in a single array */
    static seq_t *tri = NULL;    /* array of all entries so far   */
    static idx_t max_row = 0;    /* last fully computed row index*/
    static size_t capacity = 0;  /* allocated array capacity     */

    /* initialise on first call  */
    if (tri == NULL)
    {
        capacity = 1;
        tri = (seq_t *)malloc(sizeof(seq_t) * capacity);
        if (tri == NULL)
            return (seq_t)0;                       /* out of memory */
        tri[0] = (seq_t)1;                        /* row 0, element 0 */
        max_row = 0;
    }

    /* extend the triangle until it contains index n */
    while ( ((max_row + 1) * (max_row + 2)) / 2 <= n )
    {
        idx_t r = max_row + 1;                     /* next row to compute */
        idx_t prev_start = (r - 1) * r / 2;        /* index of row r‑1 start */
        idx_t curr_start = r * (r + 1) / 2;        /* index of row r    start */

        /* ensure enough capacity */
        size_t needed = (size_t)((r + 1) * (r + 2) / 2);
        if (needed > capacity)
        {
            size_t new_cap = capacity ? capacity : 1;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(tri, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;            /* out of memory */
            tri = tmp;
            capacity = new_cap;
        }

        /* first element of the new row */
        seq_t new_first;
        if ((r & 1) == 0)   /* r even: multiply previous first by -2 */
            new_first = -((seq_t)2 * tri[prev_start]);
        else                /* r odd: same as previous first */
            new_first = tri[prev_start];
        tri[curr_start] = new_first;

        /* interior elements 1 … r-1 */
        for (idx_t k = 1; k < r; ++k)
            tri[curr_start + k] =
                tri[prev_start + k - 1] - tri[prev_start + k];

        /* last element */
        tri[curr_start + r] = tri[prev_start + r - 1];

        max_row = r;                            /* update last row */
    }

    return tri[n];
}
