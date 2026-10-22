/*
 * A389907  a(n) = min(n, A054429(n)), where A054429 list each block of 2^n
 *          numbers (from 2^n to 2^(n+1) - 1) in reverse order.
 * https://oeis.org/A389907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = min(n, 3*2^floor(log2(n)) - 1 - n)
 * Range:     u8 n <= 255, u16 n <= 9210, u32 n <= 9210, u64 n <= 9210,
 *            u128 n <= 9210, big any n
 * Helpers:   floor_log2_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_log2_ull.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                  /* undefined for n < 1, return 0 */

    /* Find the block index k such that 2^k <= n < 2^(k+1). */
    idx_t k     = floor_log2_ull(n);
    idx_t pow2k = (idx_t)1 << k;          /* 2^k */

    /* Offset of n within its block. */
    idx_t offset = n - pow2k;

    /* Compute A054429(n) = 2^(k+1)-1 - offset, using modulo arithmetic
       to avoid overflow when pow2k == 2^63. */
    idx_t a054429_val = (pow2k << 1) - 1 - offset;  /* fits in idx_t */

    /* Return min(n, a054429_val) as seq_t. */
    if (n < a054429_val)
        return (seq_t)n;
    else
        return (seq_t)a054429_val;
}
