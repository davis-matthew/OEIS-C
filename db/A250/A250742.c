/*
 * A250742  T(n,k) = Number of (n+1) X (k+1) 0..1 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nonincreasing
 *          x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = 2^(k+1) + 2^(n+1) – 2 The sequence a(n) enumerates the
 *            values T(n,k) along the antidiagonals of the (n,k) grid:
 *            (1,1),(1,2),(2,1),(1,3),(2,2),(3,1),…
 * Range:     u8 n <= 21, u16 n <= 105, u32 n <= 465, u64 n <= 1450,
 *            u128 n <= 1450, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"  /* provides pow2(idx_t exp) -> seq_t */

seq_t a(idx_t n)
{
    /* Find the diagonal d such that (d-1)d/2 < n <= d(d+1)/2 */
    idx_t d = 1;
    idx_t rem = n;
    while (rem > d) {
        rem -= d;
        ++d;
    }
    /* rem is the position within diagonal d (1-based) */
    idx_t n_coord = rem;               /* row index (n in T(n,k)) */
    idx_t k_coord = d - rem + 1;       /* column index (k in T(n,k)) */

    /* Compute 2^(k+1) + 2^(n+1) – 2 */
    seq_t val = pow2(n_coord + 1);
    val += pow2(k_coord + 1);
    val -= (seq_t)2;
    return val;
}
