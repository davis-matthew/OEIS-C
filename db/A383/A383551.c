/*
 * A383551  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where A(n,k) is the number of lattice paths from (0,0)
 *          to (n,k) using steps (1,0),(0,1),(3,3).
 * https://oeis.org/A383551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,k) = Σ_{w=0}^{⌊min(n,k)/3⌋} * (n-3w + k-3w + w)! / ((n-3w)!
 *            (k-3w)! w!) * a(n) = A(i,j) where (i,j) = antidiagonal_coords(n,
 *            i, j) * * This implements a(idx_t n) for sequence A383551. * It
 *            uses the utility function antidiagonal_coords and the utility *
 *            function seq_t binom(idx_t a, idx_t b) t...
 * Range:     u8 n <= 39, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   antidiagonal_coords, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/antidiagonal_coords.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Get the (i,j) coordinates of the n-th term (by antidiagonal). */
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);

    /* Sum over all possible numbers of (3,3) steps. */
    seq_t result = 0;
    idx_t w_max = (i < j ? i : j) / 3;   /* floor(min(i,j)/3) */

    for (idx_t w = 0; w <= w_max; ++w)
    {
        idx_t u = i - 3 * w;   /* number of (1,0) steps   */
        idx_t v = j - 3 * w;   /* number of (0,1) steps   */

        /* Total steps = u + v + w = i + j - 5w */
        idx_t total = u + v + w;

        /* Multinomial coefficient:
         * (u+v+w)! / (u! v! w!)
         *      = binom(total, u) * binom(v + w, v)
         */
        seq_t term1 = binom(total, u);
        seq_t term2 = binom(v + w, v);
        seq_t term = term1 * term2;

        result += term;
    }

    return result;
}
