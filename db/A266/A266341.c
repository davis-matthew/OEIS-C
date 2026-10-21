/*
 * A266341  If A036987(n) = 1, a(n) = n - A053644(n), otherwise a(n) = n -
 *          A053644(n) + 2^(A063250(n)-1).
 * https://oeis.org/A266341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is obtained from n by removing its most significant 1‑bit
 *            and inserting it at the position of the most significant 0 among
 *            the remaining bits. If no such 0 exists the bit is dropped.
 *            a(0)=0
 * Range:     u8 n <= 511, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   floor_log2_uint64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Position of the most significant 1‑bit */
    idx_t msb_pos = floor_log2_uint64(n);
    seq_t msb_val = pow2(msb_pos);              /* 1 << msb_pos */

    /* Mask for bits below the MSB */
    seq_t lower_mask = msb_val - (seq_t)1;
    seq_t lower      = (seq_t)n & lower_mask;   /* lower bits of n */

    /* Bits that are 0 in the lower part (within the mask) */
    seq_t zeros = lower_mask - lower;           /* complement within the mask */

    if (zeros == (seq_t)0)          /* no zero bit; drop the MSB */
        return (seq_t)n - msb_val;

    /* Position of the most significant zero bit */
    idx_t pos = floor_log2_uint64((idx_t)zeros);

    return (seq_t)n - msb_val + pow2(pos);
}
