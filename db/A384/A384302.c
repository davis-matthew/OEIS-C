/*
 * A384302  a(n) = Product_{k=0..2*n-1} (3*n+k).
 * https://oeis.org/A384302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Π_{k=0}^{2*n-1} (3*n + k)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"

seq_t a(idx_t n) {
    /* Compute a(n) = product_{k=0}^{2*n-1} (3*n + k) */
    idx_t two_n = 2 * n;
    seq_t res = (seq_t)1;
    idx_t start = 3 * n;
    for (idx_t i = 0; i < two_n; ++i) {
        /* mul_small multiplies a seq_t by a small unsigned int */
        res = mul_small(res, (unsigned int)(start + i));
    }
    return res;
}
