/*
 * A385627  Table read by rows: T(n, k) = (binomial(n, k) * fibonomial(n, k))
 *          mod 2.
 * https://oeis.org/A385627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th element of the concatenation of the rows of the
 *            triangle T(n,k) = [ (n & k == k) && (n % 3 >= k % 3) && (~(n/3)
 *            & (k/3) == 0) ]. The row index and column index for the n-th
 *            element are determined by the cumulative counts of previous rows
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row and column corresponding to the global index n */
    idx_t row  = 0;
    idx_t cum  = 0;          /* number of elements before the current row */
    while (cum + row + 1 <= n)
    {
        cum += row + 1;
        row++;
    }
    idx_t col = n - cum;     /* position in the current row */

    /* Compute T(row, col) ; since we are interested only in parity,
       the multiplication reduces to logical AND. */
    if ( (row & col) != col )          return (seq_t)0;
    if ( (row % 3) < (col % 3) )       return (seq_t)0;
    if ( (~(row / 3)) & (col / 3) )    return (seq_t)0;

    return (seq_t)1;
}
