/*
 * A269946  Triangle read by rows, Lah numbers of order 3, T(n,n) = 1, T(n,k)
 *          = 0 if k<0 or k>n, otherwise T(n,k) =
 *          T(n-1,k-1)+((n-1)^3+k^3)*T(n-1, k), for n>=0 and 0<=k<=n.
 * https://oeis.org/A269946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k)=T(n-1,k-1)+((n-1)^3+k^3)*T(n-1,k) with T(0,0)=1, T(n,0)=0
 *            for n>0, T(n,n)=1; a(n) is the flattened triangle
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 36, u64 n <= 66, u128 n <= 153,
 *            big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/seq_alloc_new_array.h"   /* for seq_alloc_new_array */

seq_t a(idx_t n)
{
    /* Compute the row index t and column k such that a(n)=T(t,k). */
    idx_t t = (idx_t)((sqrt((long double)8.0L * n + 1.0L) - 1.0L) / 2.0L);
    /* Correct potential rounding errors */
    while ((t + 1) * (t + 2) / 2 <= n) ++t;
    while (t * (t + 1) / 2 > n) --t;
    idx_t tri_before = t * (t + 1) / 2;
    idx_t k = n - tri_before;            /* 0‑based index within row t */

    /* Allocate space for rows up to t (static so it survives across calls) */
    static seq_t *prev = NULL;
    static seq_t *cur  = NULL;
    static size_t prev_cap = 0;

    size_t needed = (size_t)(t + 1);
    if (prev_cap < needed) {
        /* Allocate new arrays; old ones are leaked, but t grows slowly. */
        prev = seq_alloc_new_array(needed);
        cur  = seq_alloc_new_array(needed);
        prev_cap = needed;
    }

    /* Initialise the base row (row 0).  T(0,0)=1. */
    prev[0] = (seq_t)1;

    /* Build rows 1 .. t.  Each iteration uses only two rows. */
    for (idx_t row = 1; row <= t; ++row) {
        /* T(row,0)=0 for all rows >0 */
        cur[0] = (seq_t)0;

        /* (row-1)^3 computed once per row */
        idx_t rminus = row - 1;
        seq_t cube_row = (seq_t)rminus * rminus * rminus;

        /* Compute internal entries k=1..row-1 */
        for (idx_t col = 1; col < row; ++col) {
            seq_t term1 = prev[col - 1];          /* T(row-1, col-1) */
            idx_t c = col;
            seq_t cube_col = (seq_t)c * c * c;    /* col^3 */
            seq_t mul = cube_row + cube_col;     /* ((row-1)^3 + col^3) */
            seq_t term2 = mul * prev[col];       /* times T(row-1, col) */
            cur[col] = term1 + term2;
        }

        /* T(row,row)=1 */
        cur[row] = (seq_t)1;

        /* Swap prev and cur for the next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    /* After the loop, prev holds row t. Return the requested entry. */
    return prev[k];
}
