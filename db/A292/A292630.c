/*
 * A292630  Square array A(n,k), n>=0, k>=0, read by antidiagonals, where
 *          column k is the expansion of e.g.f. exp(k*x)*(BesselI(0,2*x) +
 *          BesselI(1,2*x)).
 * https://oeis.org/A292630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 72, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A292630
   Square array A(n,k), n>=0, k>=0, read by antidiagonals.
   The entry A(n,k) equals the coefficient of xⁿ/n! in
   exp(k·x)*(BesselI₀(2x)+BesselI₁(2x)).
   A(n,k) can be computed by the closed form

        A(n,k) = Σ_{s=0}^{n} C(n,s) · C(n-s,⌊(n-s)/2⌋) · kˢ

   where C(·,·) is the binomial coefficient.
   The sequence a(n) is obtained by reading this array along the
   antidiagonals. */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Special case: n = 0 */
    if (n == 0) return (seq_t)1;

    /* Determine the diagonal d.  We want the largest d with
       d*(d+1)/2 ≤ n.  This is the row index where the n‑th
       element lies.  Use an integer binary search. */
    idx_t low = 0, high = 1;
    /* Find an upper bound for d. */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) high <<= 1;
    /* Binary search for the largest d satisfying the inequality. */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t d = low;

    /* Position along the diagonal */
    idx_t prev_tri = d * (d + 1) / 2;          /* t_{d-1} */
    idx_t pos = n - prev_tri;                /* 0 … d */

    /* Map to the cell (row, col) = (pos, d - pos) in the array */
    idx_t row = pos;
    idx_t col = d - pos;

    /* Compute A(row, col) by the closed form */
    seq_t sum  = 0;
    seq_t base = (seq_t)col;                 /* k */
    seq_t pow  = 1;                          /* k^s */
    for (idx_t s = 0; s <= row; ++s) {
        idx_t r   = row - s;
        seq_t term = binomial(row, s);        /* C(row, s)   */
        term *= binomial(r, r / 2);          /* C(r, floor(r/2)) */
        term *= pow;                         /* multiply by k^s */
        sum += term;                         /* accumulate */

        /* Update k^s for next iteration */
        if (s < row)
            pow *= base;
    }
    return sum;
}
