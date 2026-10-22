/*
 * A342850  Number of sets in the geometry determined by the Hausdorff metric
 *          at each location between two sets defined by a complete bipartite
 *          graph K(3,n) (with n at least 4) missing three edges, where all
 *          three removed edges are incident to different vertices in the
 *          3-point set and none of the removed edg...
 * https://oeis.org/A342850
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 27*7^(n-3) - 3^(n-1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 24, u128 n <= 46,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n < 4)  // undefined for n < 4 in OEIS definition
        return (seq_t)0;

    /* compute 7^(n-3) and 3^(n-1) using the pow_seq helper */
    seq_t pow7 = pow_seq((seq_t)7, (int)(n - 3));
    seq_t pow3 = pow_seq((seq_t)3, (int)(n - 1));

    /* compute 27*7^(n-3) - 3^(n-1) */
    seq_t result = (seq_t)27 * pow7 - pow3;
    return result;
}
