/*
 * A340200  Number of sets in the geometry determined by the Hausdorff metric
 *          at each location between two sets defined by a complete bipartite
 *          graph K(4,n) (with n at least 3) missing two edges, where the two
 *          removed edges are not incident to the same vertex in the 4-point
 *          set and are also not incident to t...
 * https://oeis.org/A340200
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 49*15^(n-2) - 60*7^(n-2) + 22*3^(n-2) - 2; a(3) = 379
 * Range:     u8 none, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* offset: no defined values for n < 3 */
    seq_t exp = (seq_t)(n - 2);

    seq_t p15 = big_pow((seq_t)15, exp);   /* 15^(n-2) */
    seq_t p07 = big_pow((seq_t)7,  exp);   /* 7^(n-2)  */
    seq_t p03 = big_pow((seq_t)3,  exp);   /* 3^(n-2)  */

    seq_t term1 = (seq_t)49 * p15;
    seq_t term2 = (seq_t)60 * p07;
    seq_t term3 = (seq_t)22 * p03;

    /* a(n) = term1 - term2 + term3 - 2 */
    return term1 - term2 + term3 - (seq_t)2;
}
