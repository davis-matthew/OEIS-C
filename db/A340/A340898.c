/*
 * A340898  Number of sets in the geometry determined by the Hausdorff metric
 *          at each location between two sets defined by a complete bipartite
 *          graph K(3,n) (with n at least 4) missing three edges, where all
 *          three removed edges are incident to the same vertex in the 3-point
 *          set.
 * https://oeis.org/A340898
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27*7^(n-3) - 29*3^(n-3) + 2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 24, u128 n <= 46,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)0;        /* values below the offset are undefined */
    seq_t x = (seq_t)(n - 3);
    seq_t pow7 = big_pow((seq_t)7, x);
    seq_t pow3 = big_pow((seq_t)3, x);
    seq_t term1 = (seq_t)27 * pow7;
    seq_t term2 = (seq_t)29 * pow3;
    seq_t result = term1 - term2 + (seq_t)2;
    return result;
}
