/*
 * A340434  Number of sets in the geometry determined by the Hausdorff metric
 *          at each location between two sets defined by a complete bipartite
 *          graph K(5,n) (with n at least 4) missing three edges, where all
 *          three removed edges are incident to different vertices in the
 *          5-point set but all three removed edges...
 * https://oeis.org/A340434
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*31^(n-1) - 11*15^(n-1) + 15*7^(n-1) - 3^(n+1) + 2
 * Range:     u8 none, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* For indices below the OEIS offset (n < 4) the value is not used. */
    if (n < 4)
        return 0;

    /* Compute the required powers. */
    seq_t p31 = big_pow((seq_t)31, (seq_t)(n - 1));
    seq_t p15 = big_pow((seq_t)15, (seq_t)(n - 1));
    seq_t p7  = big_pow((seq_t)7,  (seq_t)(n - 1));
    seq_t p3  = big_pow((seq_t)3,  (seq_t)(n + 1));

    /* Evaluate the formula carefully, keeping the order to avoid
       accidental underflow with unsigned arithmetic. */
    seq_t result = (seq_t)3 * p31;
    result -= (seq_t)11 * p15;
    result += (seq_t)15 * p7;
    result -= p3;
    result += (seq_t)2;

    return result;
}
