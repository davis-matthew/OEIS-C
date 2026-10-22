/*
 * A370983  Triangle read by rows: T(n, k) = (n + k - 1)! / (k!*(n - k)!) if k
 *          > 0 and T(n, 0) = 0^n.
 * https://oeis.org/A370983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where r = floor((sqrt(8*n+1)-1)/2), c = n -
 *            r*(r+1)/2, T(r,0) = (r==0?1:0) and for c>0 : T(r,c) = C(r,c) *
 *            ∏_{i=1}^{c-1} (r+i) (C(r,c) is the binomial coefficient) This
 *            gives the triangle read by rows as listed in A370983
 * Range:     u8 n <= 17, u16 n <= 32, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Handle the first element separately. */
    if (n == 0) return 1;

    /* Find the row r such that the index n lies in it. */
    idx_t row   = 0;
    idx_t start = 0;               /* start index of current row */
    while (n >= start + row + 1) { /* more elements in next row?     */
        start += row + 1;           /* move to the beginning of next row */
        row++;
    }

    idx_t col = n - start;         /* column within this row */

    if (col == 0)                   /* non‑zero rows yield 0 on the first column */
        return 0;

    /* Compute the binomial coefficient C(row,col). */
    seq_t result = binomial(row, col);

    /* Multiply by the remaining product ∏_{i=1}^{col-1} (row+i). */
    for (idx_t i = 1; i < col; ++i)
        result = result * (seq_t)(row + i);

    return result;
}
