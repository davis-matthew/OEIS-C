/*
 * A342327  Number of sets in the geometry determined by the Hausdorff metric
 *          at each location between two sets defined by a complete bipartite
 *          graph K(5,n) (with n at least 4) missing three edges, where all
 *          three removed edges are incident to different vertices in the
 *          5-point set but exactly two removed edg...
 * https://oeis.org/A342327
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 105*31^(n-2) - 185*15^(n-2) + 116*7^(n-2) - 29*3^(n-2) +
 *            2 (offset 4)
 * Range:     u8 none, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 4) return 0; /* values are defined for n ≥ 4 */

    seq_t exp = (seq_t)(n - 2);
    seq_t pow31 = big_pow((seq_t)31, exp);
    seq_t pow15 = big_pow((seq_t)15, exp);
    seq_t pow7  = big_pow((seq_t)7,  exp);
    seq_t pow3  = big_pow((seq_t)3,  exp);

    seq_t result = 105 * pow31;
    result -= 185 * pow15;
    result += 116 * pow7;
    result -= 29 * pow3;
    result += 2;

    return result;
}
