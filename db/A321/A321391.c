/*
 * A321391  Array read by antidiagonals: T(n,k) is the number of achiral rows
 *          of n colors using up to k colors.
 * https://oeis.org/A321391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   The sequence is obtained by reading the array T(i,k) along
 *            antidiagonals from top‐right to bottom‐left. For i==0, T(0,k)=1.
 *            For i>0, T(i,k)=k^ceil(i/2). The index n of the sequence
 *            corresponds to the point (i,j) on the antidiagonal where i=j and
 *            k=d−j, with d the antidiagonal number satisfying d(...
 * Range:     u8 n <= 72, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find the antidiagonal d containing index n. */
    idx_t d = 0;
    idx_t cum = 0;           /* cumulative count up to previous diagonal */
    while ((d + 1) * (d + 2) / 2 <= n) {
        d++;
    }
    cum = d * (d + 1) / 2;   /* elements before diagonal d */

    /* Position inside diagonal: j = n - cum.  i=j, k=d-j. */
    idx_t j = n - cum;       /* 0 ≤ j ≤ d */
    idx_t i = j;             /* row index */
    idx_t k = d - j;         /* column index */

    if (i == 0) {
        return (seq_t)1;
    } else {
        /* Exponent = ceil(i/2) = (i+1)/2 integer division. */
        seq_t exp = (seq_t)((i + 1) / 2);
        return big_pow((seq_t)k, exp);
    }
}
