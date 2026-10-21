/*
 * A277355  a(n) = Product_{k=1..n} (2^k + k).
 * https://oeis.org/A277355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} (2^k + k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t pow2   = 2;                /* 2^1 */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t term = pow2 + (seq_t)k;/* 2^k + k */
        result = seq_mul(result, term);
        pow2   = seq_mul(pow2, 2);  /* next power of 2 */
    }

    return result;
}
