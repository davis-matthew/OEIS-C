/*
 * A398983  Array read by ascending antidiagonals: A(n,k) =
 *          k*binomial((k+1)*n, n)/((k + 1)*n - 1), with k > 0.
 * https://oeis.org/A398983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,k) = k * binomial((k+1)*n, n) / ((k+1)*n - 1)
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is obtained by reading A(n,k) on ascending antidiagonals
   (diagonals indexed by d = n + k, increasing) with k running
   from 1 up to d-1 in each diagonal.  The implementation
   searches for the diagonal containing the nth element in O(√n). */

#include "../utils/binomial.h"

/* Find the smallest integer d ≥ 2 such that (d‑1)*d/2 ≥ n.
   n is expected to be reasonably small (≤ ~10⁶) so a simple
   linear scan is fast enough. */
static inline idx_t find_diagonal(idx_t n)
{
    idx_t d = 2;
    while (((d - 1) * d) / 2 < n)
        ++d;
    return d;
}

seq_t a(idx_t n)
{
    /* n must be ≥ 1; behavior for n==0 is undefined. */
    if (n == 0)
        return 0;

    /* Locate the diagonal d containing the nth term. */
    idx_t d = find_diagonal(n);

    /* Count of terms up to diagonal d‑1: (d‑2)*(d‑1)/2. */
    idx_t prev = d > 2 ? ((d - 2) * (d - 1)) / 2 : 0;

    /* Position inside this diagonal (1‑based). */
    idx_t pos = n - prev;          /* 1 … d‑1 */

    /* Within the diagonal, k runs from 1 to d‑1 (increasing). */
    idx_t k   = pos;               /* column index (k > 0) */
    idx_t row = d - pos;           /* row index n ≥ 1 */

    /* Compute binomial coefficient ((k+1)*row choose row). */
    idx_t mult = (k + 1) * row;    /* (k+1)*row fits in idx_t for all tested n */
    seq_t bin = binomial(mult, row);

    /* Numerator and denominator of the formula. */
    seq_t num = (seq_t)k * bin;
    seq_t den = (seq_t)((k + 1) * row - 1);

    return num / den;
}
