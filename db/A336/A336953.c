/*
 * A336953  In binary representation, rotate the digits of n right n places.
 * https://oeis.org/A336953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = rotate the binary representation of n right by n
 *            positions (mod bit length)
 * Range:     u8 n <= 259, u16 n <= 7655, u32 n <= 7655, u64 n <= 7655,
 *            u128 n <= 7655, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    /* Bit length of n – for n = 0 treat it as 1 bit. */
    idx_t len = (n == 0) ? 1 : bitlen_seq_t((seq_t)n);
    idx_t k   = n % len;

    /* No rotation needed. */
    if (k == 0)
        return (seq_t)n;

    /* Bits to be moved to the front (low k bits). */
    seq_t mask = (((seq_t)1) << k) - 1;          /* lower k bits set */
    seq_t low  = ((seq_t)n) & mask;              /* bits to rotate in   */
    seq_t high = ((seq_t)n) >> k;                /* remaining bits      */

    /* Perform the rotation: low bits move to the high end. */
    return (low << (len - k)) | high;
}
