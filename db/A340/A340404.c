/*
 * A340404  Number of sets in the geometry determined by the Hausdorff metric
 *          at each location between two sets defined by a complete bipartite
 *          graph K(5,n) (with n at least 4) missing three edges, where the
 *          removed edges are incident to different vertices in the 5-point
 *          set and none of the removed edges are...
 * https://oeis.org/A340404
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3375*31^(n-3) - 2891*15^(n-3) + 846*7^(n-3) - 10*3^(n-1)
 *            + 2
 * Range:     u8 none, u16 none, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*            for all n >= 4 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Undefined for indices below the OEIS offset. */
    if (n < 4)
        return 0;

    /* Compute the power terms using the provided big_pow utility. */
    seq_t p31 = big_pow((seq_t)31, (seq_t)(n - 3));
    seq_t p15 = big_pow((seq_t)15, (seq_t)(n - 3));
    seq_t p07 = big_pow((seq_t)7,  (seq_t)(n - 3));
    seq_t p03 = big_pow((seq_t)3,  (seq_t)(n - 1));

    /* Assemble the formula term by term. */
    seq_t term1 = p31 * (seq_t)3375;
    seq_t term2 = p15 * (seq_t)2891;
    seq_t term3 = p07 * (seq_t)846;
    seq_t term4 = p03 * (seq_t)10;

    /* Compute the final expression carefully with unsigned arithmetic. */
    seq_t res = term1 - term2 + term3 - term4 + (seq_t)2;
    return res;
}
