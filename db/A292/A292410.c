/*
 * A292410  Difference between (2n+1)^2 and highest power of 2 less than or
 *          equal to (2n+1)^2.
 * https://oeis.org/A292410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = (2*n+1)^2 - 2^floor(log2((2*n+1)^2))
 * Range:     u8 n <= 7, u16 n <= 220, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bitlen_seq_t.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute (2*n+1)^2 */
    seq_t k = ((seq_t)n) * 2 + 1;   /* k = 2*n + 1   */
    seq_t m = k * k;                /* m = k^2       */

    /* Find the largest power of two ≤ m */
    unsigned bitlen = bitlen_seq_t(m);          /* bitlen = floor(log2(m)) + 1 */
    seq_t p = pow2((idx_t)(bitlen - 1));        /* p = 2^(bitlen-1) <= m */

    /* Return the difference */
    return m - p;
}
