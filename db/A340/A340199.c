/*
 * A340199  Number of sets in the geometry determined by the Hausdorff metric
 *          at each location between two sets defined by a complete bipartite
 *          graph K(3,n) (with n at least 3) missing two edges, where the two
 *          removed edges are not incident to the same vertex in the 3-point
 *          set and are also not incident to t...
 * https://oeis.org/A340199
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9*7^(n-2) - 7*3^(n-2) + 1; offset 3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n) {
    if (n < 3) return 0;
    idx_t exp = n - 2;
    seq_t pow7 = pow_int((seq_t)7, exp);
    seq_t pow3 = pow_int((seq_t)3, exp);
    seq_t res = (seq_t)9 * pow7 - (seq_t)7 * pow3 + (seq_t)1;
    return res;
}
