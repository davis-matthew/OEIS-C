/*
 * A370427  a(n) is the least k >= 0 such that n OR k is a binary palindrome
 *          (where OR denotes the bitwise OR operator).
 * https://oeis.org/A370427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = the minimum k ≥ 0 such that (n OR k) is a binary *
 *            palindrome, with k & n = 0. The function computes this by *
 *            constructing the smallest binary palindrome m that is ≥ n and *
 *            contains all 1‑bits of n, then returns k = m – n
 * Range:     u8 n <= 513, u16 n <= 9276, u32 n <= 9276, u64 n <= 9276,
 *            u128 n <= 9276, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* External integer types supplied by the build system */
extern idx_t _floor_isqrt_u64(idx_t n);

seq_t a(idx_t n)
{
    /* n = 0 is a special case – 0 itself is already a palindrome */
    if (n == 0)
        return (seq_t)0;

    /* Compute the bit length L of n (number of bits needed to represent n) */
    idx_t L = 0;
    idx_t tmp = n;
    while (tmp) {
        ++L;
        tmp >>= 1;
    }

    /* Build m by mirroring each 1‑bit of n across the center */
    seq_t m = (seq_t)0;
    for (idx_t i = 0; i < L; ++i) {
        if ((n >> i) & 1)
        {
            m |= ((seq_t)1 << i);
            m |= ((seq_t)1 << (L - 1 - i));
        }
    }

    /* Since m and n have disjoint 1‑bits, m = n + k where k = m – n */
    return m - n;
}
