/*
 * A317023  Square array A(n,k), n >= 0, k >= 0, read by ascending
 *          antidiagonals, where the sequence of row n is the expansion of
 *          (1-x^(n+1))/((1-x)^(n+1)).
 * https://oeis.org/A317023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   Let d be the smallest non‑negative integer with (d+1)(d+2)/2 > n.
 *   Let pos = n - d(d+1)/2,      (0 ≤ pos ≤ d)
 *   Let n'   = d - pos,  k   = pos.
 *   Then a(n) = binomial(d, k) - binomial(k-1, n') ,
 *   interpreting binomial(i,j)=0 if i<j or j<0.
 *
 * The function below uses the provided binomial utility.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* find d by incremental search; d is at most √(2*n) */
    idx_t d = 0;
    while ( ((unsigned __int128)(d + 1) * (d + 2) / 2) <= n )
        ++d;

    idx_t prev  = (unsigned __int128)d * (d + 1) / 2;
    idx_t pos   = n - prev;          /* position inside diagonal */
    idx_t k     = pos;
    idx_t nprime = d - pos;

    seq_t term1 = binomial(d, k);
    seq_t term2 = 0;
    if (k > 0 && nprime <= k - 1)
        term2 = binomial(k - 1, nprime);

    return term1 - term2;
}
