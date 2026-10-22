/*
 * A340436  Number of sets in the geometry determined by the Hausdorff metric
 *          at each location between two sets defined by a complete bipartite
 *          graph K(3,n) (with n at least 4) missing three edges, where
 *          exactly two of the removed edges are incident to the same vertex
 *          in the 3-point set but none of the remov...
 * https://oeis.org/A340436
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 27*7^(n-3) - 13*3^(n-3) + 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 24, u128 n <= 46,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* For n < 4 the sequence is not defined; return 0 */
    if (n < 4)
        return (seq_t)0;

    /* exponent is n-3 */
    seq_t exp = (seq_t)(n - 3);

    /* compute powers */
    seq_t p7 = big_pow((seq_t)7, exp);
    seq_t p3 = big_pow((seq_t)3, exp);

    /* compute the formula */
    seq_t res = (seq_t)27 * p7;
    res -= (seq_t)13 * p3;
    res += (seq_t)1;

    return res;
}
