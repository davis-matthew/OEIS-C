/*
 * A343374  Number of sets in the geometry determined by the Hausdorff metric
 *          at each location between two sets defined by a complete bipartite
 *          graph K(5,n) (with n at least 4) missing three edges, where
 *          exactly two removed edges are incident to the same vertex in the
 *          5-point set and exactly two removed edge...
 * https://oeis.org/A343374
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 105*31^(n-2) - 217*15^(n-2) + 148*7^(n-2) - 13*3^(n-1) +
 *            3, with offset 4 (a(4) is the first term)
 * Range:     u8 none, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n < 4) return 0;          /* behavior below the offset is arbitrary */

    idx_t e = n - 2;              /* exponent for the first three powers */
    seq_t p31 = big_pow((seq_t)31, (seq_t)e);
    seq_t p15 = big_pow((seq_t)15, (seq_t)e);
    seq_t p07 = big_pow((seq_t)7,  (seq_t)e);
    seq_t p03 = big_pow((seq_t)3,  (seq_t)(n - 1));

    seq_t term1 = p31 * (seq_t)105;
    seq_t term2 = p15 * (seq_t)217;
    seq_t term3 = p07 * (seq_t)148;
    seq_t term4 = p03 * (seq_t)13;

    seq_t res = term1;
    res -= term2;
    res += term3;
    res -= term4;
    res += (seq_t)3;

    return res;
}
