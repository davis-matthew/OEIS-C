/*
 * A381839  In the binary expansion of n (without leading zeros): complement
 *          the bits strictly between the leftmost and the rightmost 0's, if
 *          any.
 * https://oeis.org/A381839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A381839 – binary-complement‑between‑zeros permutation
 * a(n) = n,  if the binary representation of n contains fewer than two zeros;
 * otherwise a(n) = n XOR mask, where mask has ones in the positions strictly
 * between the leftmost and rightmost zero bits of n.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* single zero has no interior bits */

    /* Compute bit length of n (position of most significant 1 + 1) */
    idx_t len = 0;
    idx_t tmp = n;
    while (tmp) {
        len++;
        tmp >>= 1;
    }

    /* Find the rightmost zero (least significant zero) */
    idx_t right = 0;
    while (right < len && ((n >> right) & 1u)) right++;
    if (right == len) /* no zero bits */
        return (seq_t)n;

    /* Find the leftmost zero (most significant zero) */
    idx_t leftmost_zero = len - 1;
    while (((n >> leftmost_zero) & 1u) == 1u) leftmost_zero--;

    /* If there is a contiguous block of bits between the two zeros,
       complement them using XOR with a mask. */
    seq_t mask = 0;
    if (leftmost_zero > right + 1) {
        idx_t start = right + 1;
        idx_t end   = leftmost_zero - 1;
        idx_t i = start;
        while (i <= end) {
            mask |= ((seq_t)1 << i);
            i++;
        }
    }
    return (seq_t)n ^ mask;
}
