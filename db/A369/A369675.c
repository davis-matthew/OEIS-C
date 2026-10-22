/*
 * A369675  a(n) = Product_{k=0..n} (4^k + 4^(n-k)).
 * https://oeis.org/A369675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (4^k + 4^(n-k))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    seq_t left   = 1;
    seq_t right  = pow2(2 * n);   /* 4^n = 2^(2n) */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = left + right;   /* 4^k + 4^(n-k) */
        result *= term;
        left   *= 4;                 /* next 4^k */
        right  /= 4;                 /* next 4^(n-k) */
    }
    return result;
}
