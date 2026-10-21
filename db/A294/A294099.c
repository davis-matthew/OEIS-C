/*
 * A294099  Rectangular array read by (upward) antidiagonals: A(n,k) =
 *          Sum_{j=0..k} (-1)^floor(j/2)*binomial(k-floor((j+1)/2),
 *          floor(j/2))*n^(k-j), n >= 1, k >= 0.
 * https://oeis.org/A294099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is read from the array A(n,k) defined by A(n,0)=1,
 *            A(n,1)=n+1, A(n,k)=n*A(n,k-1)-A(n,k-2). The sequence a(n)
 *            enumerates the array entries by upward antidiagonals: row n' =
 *            d+2-r, column k' = r-1, where d is the diagonal number
 *            satisfying d(d+1)/2 < n ≤ (d+1)(d+2)/2, and r = n - d(d+1)/2
 * Range:     i8 n <= 31, i16 n <= 72, i32 n <= 181, i64 n <= 426,
 *            i128 n <= 1160, big any n
 * Helpers:   compute_A_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/compute_A_row.h"

seq_t a(idx_t n)
{
    /* Find the diagonal d such that d(d+1)/2 < n <= (d+1)(d+2)/2. */
    idx_t d = 0;
    idx_t sum = 0;
    while (sum < n) {
        sum += d + 1; /* add the number of terms in diagonal d */
        ++d;
    }
    /* After loop d is one more than the desired diagonal. */
    --d;                     /* now d is the diagonal index (0-based) */
    idx_t prevSum = d * (d + 1) / 2;          /* terms before this diagonal */
    idx_t r = n - prevSum;                    /* 1-based position inside the diagonal */
    /* Map to array coordinates: n' = d+2-r, k' = r-1. */
    seq_t row = (seq_t)(d + 2 - r);   /* n' (row index, >=1) */
    idx_t col = r - 1;                /* k' (column index, >=0) */
    return compute_A_row(row, col);
}
