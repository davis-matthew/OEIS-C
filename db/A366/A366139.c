/*
 * A366139  In binary representation, rotate the digits of n left n places.
 * https://oeis.org/A366139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = rotate left the binary representation of n by n positions
 * Range:     u8 n <= 258, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 86 terms through n = 9764 (2026-11-01)
 * Helpers:   ilog2_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ilog2_idx.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Number of bits needed to represent n, at least 1. */
    idx_t k = ilog2_idx(n) + 1;

    /* Rotation amount modulo the bit‑length. */
    idx_t r = n % k;
    if (r == 0) return (seq_t)n;

    /* Perform the rotation in a wide type to avoid undefined shifts. */
    idx_t left  = ((idx_t)n << r);
    idx_t right = ((idx_t)n >> (k - r));
    idx_t mask  = (k == 64) ? ~(idx_t)0 : (((idx_t)1 << k) - 1);
    idx_t res   = (left | right) & mask;

    return (seq_t)res;
}
