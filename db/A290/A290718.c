/*
 * A290718  a(n) = 2^(n + 1) + 4^(n - 1) - 2.
 * https://oeis.org/A290718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n + 1) + 4^(n - 1) - 2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;
    /* compute 2^(n+1) and 4^(n-1) using the provided helpers */
    seq_t term1 = pow2((idx_t)(n + 1));     /* 2^(n+1) */
    seq_t term2 = pow4((idx_t)(n - 1));     /* 4^(n-1) */
    return term1 + term2 - 2;
}
