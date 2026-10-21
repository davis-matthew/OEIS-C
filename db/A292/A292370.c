/*
 * A292370  A binary encoding of the zeros in base-4 representation of n.
 * https://oeis.org/A292370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = binary mask of positions where the base‑4 representation
 *            of n has a zero digit; a(0) = 0. The least significant base‑4
 *            digit corresponds to bit 0 of a(n)
 * Range:     u8 n <= 9546, u16 n <= 9546, u32 n <= 9546, u64 n <= 9546,
 *            u128 n <= 9546, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Special case: 0 has no digits in the defined representation. */
    if (n == 0)
        return 0;

    seq_t result = 0;
    idx_t pos = 0;
    /* Width of the output type in bits.  Shifting beyond this
       would be undefined, so we stop once we have processed all
       possible bits that can be represented. */
    const size_t width = sizeof(seq_t) * 8;

    while (n > 0)
    {
        /* If the least significant base‑4 digit is zero, set the bit. */
        if ((n & 3ULL) == 0)
        {
            if ((size_t)pos < width)
                result |= ((seq_t)1 << pos);
            /* If pos >= width, the required bit would not fit in
               the current seq_t type; further bits are ignored
               because the value would overflow the type anyway. */
        }
        /* Shift n right by two decimal places (divide by 4). */
        n >>= 2;
        ++pos;
    }
    return result;
}
