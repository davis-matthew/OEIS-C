/*
 * A273105  a(n) = A038572(n) + A006257(n), sum of the two numbers obtained by
 *          rotating the binary representation of n by one place to the right
 *          and to the left.
 * https://oeis.org/A273105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A038572(n) + A006257(n)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) = 0 */
    if (n == 0) return 0;

    /* use a native 64‑bit unsigned for the bit manipulations */
    uint64_t nn = (uint64_t)n;

    /* compute the number of bits in the binary representation of n */
    unsigned bits = 0;
    uint64_t tmp = nn;
    while (tmp) { bits++; tmp >>= 1; }

    /* rotate right by one */
    uint64_t right = (nn >> 1) | ((nn & 1ULL) << (bits - 1));

    /* rotate left by one (the wrapped bit is forced into low position) */
    uint64_t left = ((nn << 1) | (nn >> (bits - 1)));

    /* mask to discard any bits that overflow the original width */
    uint64_t mask = (bits == 64) ? ~0ULL : ((1ULL << bits) - 1);
    left &= mask;

    /* return the sum cast back to the required seq_t type */
    return (seq_t)(left + right);
}
