/*
 * A357824  Total number A(n,k) of k-tuples of semi-Dyck paths from (0,0) to
 *          (n,n-2*j) for j=0..floor(n/2); square array A(n,k), n>=0, k>=0,
 *          read by antidiagonals.
 * https://oeis.org/A357824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sum_{j=0..⌊row/2⌋} (C(row,j)-C(row,j-1))^k where (row,k)
 *            are the square‑array coordinates obtained from the antidiagonal
 *            index n
 * Range:     u8 n <= 48, u16 n <= 72, u32 n <= 127, u64 n <= 219,
 *            u128 n <= 390, big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/pow_seq.h"

/* Compute the entry A(row,k) of the square array.  */
static seq_t A_rowcol(idx_t row, idx_t k)
{
    /* If k==0 the summands are all 1, so the result is the number
       of terms: floor(row/2)+1.  */
    if (k == 0)
        return (seq_t)(row / 2 + 1);

    idx_t max_j = row / 2;
    seq_t sum = 0;

    seq_t binom = 1; /* C(row,0) */
    seq_t prev  = 0; /* C(row,-1) = 0 */

    for (idx_t j = 0; j <= max_j; ++j)
    {
        seq_t diff = binom - prev;               /* C(row,j)-C(row,j-1) */
        seq_t pw   = pow_seq(diff, (int)k);      /* diff^k */
        sum += pw;

        if (j == max_j)
            break;

        prev  = binom;
        binom = (binom * (row - j)) / (j + 1);   /* C(row,j+1) */
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Determine the antidiagonal d for which d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    idx_t d = (idx_t)((sqrt((double)(8 * n + 1)) - 1) / 2);
    /* Correct possible rounding errors */
    while (d * (d + 1) / 2 > n) --d;
    while ((d + 1) * (d + 2) / 2 <= n) ++d;

    idx_t tri = d * (d + 1) / 2;   /* first index of this diagonal */
    idx_t p   = n - tri;           /* position within the diagonal */

    idx_t row = p;                 /* row index of the square array   */
    idx_t k   = d - p;             /* column index of the square array */

    return A_rowcol(row, k);
}
