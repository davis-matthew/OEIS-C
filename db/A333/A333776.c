/*
 * A333776  Scan the binary representation of n from right to left; at each 1,
 *          reverse the bits to the right and excluding this 1. The resulting
 *          binary representation is that of a(n).
 * https://oeis.org/A333776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   reverse_lower_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is obtained by scanning the binary representation of n from
 * right to left; whenever a '1' is encountered the bits to its right
 * are reversed.  This function implements that definition directly. */
#include <stdint.h>
#include "../utils/reverse_lower_bits.h"

seq_t a(idx_t n)
{
    seq_t res = (seq_t)n;
    idx_t i = 0;
    while ((n >> i) != 0) {
        if ((n >> i) & 1)
            res = reverse_lower_bits(res, (unsigned int)i);
        ++i;
    }
    return res;
}
