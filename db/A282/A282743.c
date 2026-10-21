/*
 * A282743  Irregular triangle read by rows, giving coefficients arising when
 *          solving g(n) = f(n)+ f(n-1) + f(n-2) for f(n).
 * https://oeis.org/A282743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = r - ⌊3·d/2⌋ * where r is the unique row index such that
 *            the * cumulative number of elements of rows 0..r‑1 is ≤ n * and
 *            the following row r contains the element at * position d
 *            (0‑based) within that row. * * Row r has L(r) = ⌊(2r+1)/3⌋ + 1
 *            elements, * and the j‑th element of that row (j =...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* number of elements in row r */
static inline idx_t row_length(idx_t r)
{
    /* L(r) = floor((2r+1)/3) + 1 */
    return ( (2*r + 1) / 3 ) + 1;
}

seq_t a(idx_t n)
{
    idx_t pos = 0;   /* running total of indices processed */
    idx_t r   = 0;   /* current row */

    /* find the row containing index n */
    for ( ; ; ++r)
    {
        idx_t len = row_length(r);
        if (pos + len > n)
            break;
        pos += len;
    }

    /* offset of n within the found row */
    idx_t d = n - pos;

    /* compute the value at this position */
    return (seq_t)r - (seq_t)((3*d) / 2);
}
