/*
 * A376322  (1/4) times obverse convolution (2)**(2^n + 1); see Comments.
 * https://oeis.org/A376322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = ∏_{k=1..n} (2^k + 3) for n ≥ 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp) */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t result = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t factor = pow2(k) + (seq_t)3;
        result *= factor;
    }
    return result;
}
