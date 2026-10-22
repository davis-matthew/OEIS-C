/*
 * A335612  Number of sets (in the Hausdorff metric geometry) at each location
 *          between two sets defined by a complete bipartite graph K(3,n)
 *          (with n at least 3) missing two edges, where the removed edges are
 *          incident to the same vertex in the three point part.
 * https://oeis.org/A335612
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9*7^(n-2) - 11*3^(n-2) + 2
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
        return 0; /* offset is 3; values below are undefined */

    idx_t exp = n - 2;            /* exponent for the powers */
    seq_t pow7 = big_pow((seq_t)7, (seq_t)exp);
    seq_t pow3 = big_pow((seq_t)3, (seq_t)exp);

    seq_t result = ((seq_t)9) * pow7 - ((seq_t)11) * pow3 + (seq_t)2;
    return result;
}
