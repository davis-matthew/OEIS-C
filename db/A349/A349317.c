/*
 * A349317  Triangle T(n,k): T(n,k) = 1 if gcd(n, k) > 1, else 0.
 * https://oeis.org/A349317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9548, u16 n <= 9548, u32 n <= 9548, u64 n <= 9548,
 *            u128 n <= 9548, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 *  OEIS A349317 (flattened triangle of T(n,k)):
 *  a(n) = 1  if gcd(i,j) > 1,
 *  a(n) = 0  otherwise,
 *  where i is the row containing the n-th element
 *  (minimal i such that i(i+1)/2 ≥ n) and
 *  j = n – (i-1)i/2 is the column index in that row.
 *
 *  The function evaluates the characteristic function of
 *  the triangle T(i,j) = [gcd(i,j) > 1].
 */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Find the row i that contains the n‑th element. */
    idx_t i = 1;
    while (i * (i + 1) / 2 < n)
        ++i;

    /* Column index j within row i. */
    idx_t j = n - ((i - 1) * i / 2);

    /* Compute gcd of i and j using the supplied big‑int gcd. */
    seq_t g = gcd((seq_t)i, (seq_t)j);

    /* Return 1 if the gcd is greater than 1, else 0. */
    return (g > (seq_t)1) ? (seq_t)1 : (seq_t)0;
}
