/*
 * A380934  Elias delta encoding of n converted from base 2 to integer.
 * https://oeis.org/A380934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Elias δ‑encoding of n converted from base 2 to integer
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the bit length L of n */
    idx_t L = 0;
    idx_t tmp = n;
    while (tmp) {
        ++L;
        tmp >>= 1;
    }

    /* Build the Elias δ code as an integer */
    seq_t res = 0;

    /* Add the zero prefix of the γ‑encoded length (L) */
    for (idx_t i = 0; i < L - 1; ++i)
        res <<= 1;          /* shifting a zero bit */

    /* Append the binary representation of L */
    for (idx_t i = L; i > 0; --i) {
        seq_t bit = (seq_t)((L >> (i - 1)) & 1);
        res = (res << 1) | bit;
    }

    /* Append the lower L−1 bits of n (n without the leading 1) */
    if (L > 1) {
        seq_t mask = ((seq_t)1 << (L - 1)) - 1;
        seq_t lower = ((seq_t)n) & mask;
        res = (res << (L - 1)) | lower;
    }

    return res;
}
