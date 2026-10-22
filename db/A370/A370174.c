/*
 * A370174  Triangle read by rows: Riordan array (1/(1 - x), x*(1 + x)/(1 - x
 *          - x^2)).
 * https://oeis.org/A370174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   idx_flat
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows: Riordan array (1/(1 - x), x*(1 + x)/(1 - x - x^2)) */
/* a(n) = T(n, k) where (n, k) are obtained by row‑major flattening of the triangle. */
/* Recurrence: T(n,k) = T(n-1,k) + T(n-1,k-1) + T(n-2,k) + T(n-2,k-1) with
   T(n,0)=1 and T(n,k)=0 when k>n. */
#include <stdlib.h>
#include "../utils/idx_flat.h"

static seq_t *triangle = NULL;   /* flat storage of all computed rows */
static idx_t  max_row   = 0;      /* highest row that has been computed */

seq_t a(idx_t n)
{
    /* lazy initialization of the very first entry (row 0, col 0) */
    if (triangle == NULL)
    {
        triangle = (seq_t *)malloc(sizeof(seq_t));
        triangle[0] = (seq_t)1;  /* T(0,0) */
        max_row   = 0;
    }

    /* Find the row r such that the flattened index n lies in row r */
    idx_t r = 0;
    while ( ((r + 1ULL) * (r + 2ULL) / 2ULL) <= n )
        ++r;

    /* Column within the row */
    idx_t k = n - (r * (r + 1ULL) / 2ULL);

    /* Grow the triangle if necessary, computing missing rows */
    if (r > max_row)
    {
        /* new capacity needed for rows 0..r */
        size_t new_size = (size_t)(r + 1ULL) * (r + 2ULL) / 2ULL;
        triangle = (seq_t *)realloc(triangle, new_size * sizeof(seq_t));

        /* Compute rows from max_row+1 up to r */
        for (idx_t i = max_row + 1; i <= r; ++i)
        {
            size_t row_start = idx_flat(i, 0); /* index of T(i,0) */
            for (idx_t j = 0; j <= i; ++j)
            {
                seq_t val;
                if (j == 0)
                {
                    val = (seq_t)1;   /* T(i,0) = 1 */
                }
                else
                {
                    val = (seq_t)0;
                    /* T(i-1,j) */
                    if (j <= i - 1)
                        val += triangle[idx_flat(i - 1, j)];
                    /* T(i-1,j-1) */
                    if (j > 0)
                        val += triangle[idx_flat(i - 1, j - 1)];
                    /* T(i-2,j) */
                    if (i >= 2 && j <= i - 2)
                        val += triangle[idx_flat(i - 2, j)];
                    /* T(i-2,j-1) */
                    if (i >= 2 && j > 0 && j - 1 <= i - 2)
                        val += triangle[idx_flat(i - 2, j - 1)];
                }
                triangle[row_start + j] = val;
            }
        }
        max_row = r;
    }

    /* Return the requested element */
    return triangle[idx_flat(r, k)];
}
