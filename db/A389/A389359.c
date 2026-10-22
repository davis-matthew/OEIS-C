/*
 * A389359  Triangle read by rows: T(n,0) = 1, T(n,n) = n+1 and T(n,k) = T(n,
 *          k-1) + T(n-1, k-1) + T(n-2, k-1) +1 for 0 < k < n.
 * https://oeis.org/A389359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 88, u32 n <= 296, u64 n <= 987,
 *            u128 n <= 3651, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Implementation of OEIS A389359 (triangle read by rows).
   T(n,0)   = 1
   T(n,n)   = n+1
   T(n,k)   = T(n,k-1) + T(n-1,k-1) + T(n-2,k-1) + 1   (0 < k < n)
   a(n) is the row‑wise reading of T (first T(0,0), then T(1,0),T(1,1),T(2,0),…).
*/

#include <stdlib.h>
#include <math.h>
#include <stdint.h>

/* Static storage for rows of the triangle */
static seq_t **rows = NULL;          /* array of pointers to rows */
static size_t  rows_count = 0;       /* number of rows currently stored */
static size_t  rows_cap   = 0;       /* capacity of the rows array */

/* Allocate a row of length len (in seq_t units) */
static seq_t *alloc_row(size_t len)
{
    return (seq_t*)calloc(len, sizeof(seq_t));
}

/* Build triangle rows up to (and including) row rmax */
static void build_up_to(idx_t rmax)
{
    while (rows_count <= rmax) {
        idx_t i     = rows_count;
        seq_t *row  = alloc_row((size_t)(i + 1));

        /* T(i,0) = 1 */
        row[0] = (seq_t)1U;

        /* T(i,i) = i+1 */
        if (i > 0) {
            row[i] = (seq_t)(i + 1U);
        }

        /* For 0 < k < i compute recurrence */
        if (i >= 2) {
            for (idx_t k = 1; k < i; ++k) {
                row[k] = row[k-1] + rows[i-1][k-1] + rows[i-2][k-1] + (seq_t)1U;
            }
        }

        /* Ensure rows array has enough capacity */
        if (rows_count == rows_cap) {
            size_t new_cap = rows_cap ? rows_cap * 2 : 1;
            seq_t **tmp = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
            rows = tmp;
            rows_cap = new_cap;
        }
        rows[rows_count++] = row;
    }
}

/* Return the nth term of A389359 */
seq_t a(idx_t n)
{
    /* Find row r: r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r;
    {
        /* Use long‑double sqrt for initial approximation */
        unsigned __int128 val = (unsigned __int128)8 * n + 1;
        long double d = sqrtl((long double)val);
        r = (idx_t)((d - 1.0L) / 2.0L);
    }
    /* Correct any rounding error */
    while ((unsigned __int128)(r+1)*(r+2)/2 <= n) r++;
    while ((unsigned __int128)r*(r+1)/2 > n) r--;

    /* Build rows up to r */
    build_up_to(r);

    /* Column index within the row */
    idx_t prev_sum = (unsigned __int128)r * (r + 1) / 2;
    idx_t col = n - prev_sum;

    return rows[r][col];
}
