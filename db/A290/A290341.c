/*
 * A290341  Product of k in [1, 2n-1] with k and k+1 coprime to 2n-1.
 * https://oeis.org/A290341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1}^{2n-1} k, for all k with gcd(k,2n-1)=1 and
 *            gcd(k+1,2n-1)=1
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t product = 1;
    seq_t m = (seq_t)n * 2u - 1u;
    for (idx_t k = 1; k <= 2*n - 1; ++k) {
        seq_t ks = (seq_t)k;
        if (gcd(ks, m) == 1 && gcd(ks + 1, m) == 1) {
            product *= ks;
        }
    }
    return product;
}
