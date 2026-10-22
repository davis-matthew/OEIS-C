/*
 * A337418  Number of sets (in the Hausdorff metric geometry) at each location
 *          between two sets defined by a complete bipartite graph K(3,n)
 *          (with n at least 3) missing two edges, where the removed edges are
 *          not incident to the same vertex in the 3 point part but are
 *          incident to the same vertex in the other...
 * https://oeis.org/A337418
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 7^(n-1) - 2*3^(n-1) + 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;          /* undefined for n < 3, but not used */

    seq_t exp   = (seq_t)(n - 1);
    seq_t p7    = big_pow((seq_t)7, exp);
    seq_t p3    = big_pow((seq_t)3, exp);
    seq_t two   = (seq_t)2;

    return p7 - two * p3 + (seq_t)1;
}
