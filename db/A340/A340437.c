/*
 * A340437  Number of sets in the geometry determined by the Hausdorff metric
 *          at each location between two sets defined by a complete bipartite
 *          graph K(4,n) (with n at least 4) missing three edges, where
 *          exactly two of the removed edges are incident to the same vertex
 *          in the 4-point set but none of the remov...
 * https://oeis.org/A340437
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 343*15^(n-3) - 264*7^(n-3) + 52*3^(n-3) - 2
 * Range:     u8 none, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;
    seq_t e = (seq_t)(n - 3);
    seq_t p15 = big_pow((seq_t)15, e);
    seq_t p7  = big_pow((seq_t)7,  e);
    seq_t p3  = big_pow((seq_t)3,  e);
    seq_t res = (seq_t)343 * p15 - (seq_t)264 * p7 + (seq_t)52 * p3 - (seq_t)2;
    return res;
}
