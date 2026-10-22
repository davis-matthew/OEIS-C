/*
 * A340201  Number of sets in the geometry determined by the Hausdorff metric
 *          at each location between two sets defined by a complete bipartite
 *          graph K(5,n) (with n at least 3) missing two edges, where the two
 *          removed edges are not incident to the same vertex in the 5-point
 *          set and are also not incident to t...
 * https://oeis.org/A340201
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 225*31^(n-2) - 357*15^(n-2) + 202*7^(n-2) - 46*3^(n-2) +
 *            3, for n >= 3
 * Range:     u8 none, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0; /* undefined for indices before the offset */

    int e = (int)(n - 2);          /* exponent fits in int for expected n */

    seq_t p31 = pow_seq((seq_t)31, e);
    seq_t p15 = pow_seq((seq_t)15, e);
    seq_t p7  = pow_seq((seq_t)7,  e);
    seq_t p3  = pow_seq((seq_t)3,  e);

    seq_t result = (seq_t)225 * p31
                 - (seq_t)357 * p15
                 + (seq_t)202 * p7
                 - (seq_t)46  * p3
                 + (seq_t)3;

    return result;
}
