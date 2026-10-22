/*
 * A341552  Number of sets in the geometry determined by the Hausdorff metric
 *          at each location between two sets defined by a complete bipartite
 *          graph K(3,n) (with n at least 4) missing three edges, where all
 *          three removed edges are incident to different vertices in the 3
 *          point set but exactly two removed edg...
 * https://oeis.org/A341552
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*7^(n-2)-2*3^(n-2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 4) return 0;                     /* values for n<4 are undefined */
    idx_t e = n - 2;                         /* exponent (n-2) */
    seq_t pow7 = big_pow((seq_t)7, (seq_t)e);
    seq_t pow3 = big_pow((seq_t)3, (seq_t)e);
    return (seq_t)3 * pow7 - (seq_t)2 * pow3;
}
