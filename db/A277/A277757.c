/*
 * A277757  a(n) = 2^(6*n + 1).
 * https://oeis.org/A277757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(6*n + 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"  /* Provides seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    /* Compute 6*n, taking care with idx_t multiplication. */
    idx_t exp = 6 * n;

    /* 2^(6*n) */
    seq_t val = pow2(exp);

    /* Multiply by 2 to get 2^(6*n + 1) */
    return val * (seq_t)2;
}
