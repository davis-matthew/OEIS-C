/*
 * A361045  Array read by descending antidiagonals. A(n, k) is, if n > 0, the
 *          number of multiset combinations of {0, 1} whose type is defined in
 *          the comments. A(0, k) = k + 1.
 * https://oeis.org/A361045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,k) = Sum_{j=0..k} (j*n + 1)*((k-j)*n + 1) for n>0; A(0,k) =
 *            k+1. Sequence a(n) is read from the array A(n,k) by descending
 *            antidiagonals. Indexing convention follows OEIS: a(0) = A(0,0),
 *            a(1) = A(0,1), a(2) = A(1,0),
 * Range:     u8 n <= 38, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   A_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   // for standard integer types used in helper functions
#include "../utils/A_value.h"

/* Convert global index n to coordinates (row, col) in the array A by
   descending antidiagonals.  The antidiagonals are numbered by d so that
   for d=0 we have A(0,0); for d=1 we have A(0,1),A(1,0); for d=2 we have
   A(0,2),A(1,1),A(2,0), and so on.  This routine finds the diagonal d
   and then the offset on that diagonal. */
static void coords_from_index(idx_t n, idx_t *row, idx_t *col)
{
    /* Find largest d such that d*(d+1)/2 <= n */
    idx_t d = 0;
    while ( ((d + 1) * (d + 2) / 2) <= n )
        ++d;

    idx_t tri = d * (d + 1) / 2;      /* triangular number for d          */
    idx_t offset = n - tri;           /* position within the diagonal     */

    *row = offset;                    /* row index                         */
    *col = d - offset;                /* column index                      */
}

/* Main function required by the framework */
seq_t a(idx_t n)
{
    idx_t row, col;
    coords_from_index(n, &row, &col);
    return A_value(row, col);
}
