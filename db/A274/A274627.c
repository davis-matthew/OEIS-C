/*
 * A274627  Product_{i=0..3} (2^floor((n+i)/4)-1).
 * https://oeis.org/A274627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prod_{i=0..3} (2^floor((n+i)/4)-1)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 32, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 0; i < 4; ++i) {
        idx_t k = (n + i) / 4;          /* floor((n+i)/4) */
        seq_t term = pow2(k) - 1;       /* (2^k - 1) */
        result *= term;                 /* multiply the four factors */
    }
    return result;
}
