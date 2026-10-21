/*
 * A241717  The number of P-positions in the game of Nim with up to 3 piles,
 *          allowing for piles of zero, such that the number of objects in the
 *          largest pile is n.
 * https://oeis.org/A241717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*(n+1-2^⌊log2 n⌋) - 3 for n > 0, a(0) = 1
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A241717 (P-positions in Nim with up to 3 piles) */
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;

    /* b = floor(log2(n)) */
    idx_t b = floor_log2_uint64(n);

    /* 2^b as seq_t */
    seq_t pow2b = ((seq_t)1 << b);

    /* c = n + 1 - 2^b */
    seq_t c = (seq_t)n + (seq_t)1 - pow2b;

    /* a(n) = 6*c - 3 */
    return (seq_t)6 * c - (seq_t)3;
}
