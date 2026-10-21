/*
 * A253315  a(n) = bitwise XOR of all the bit numbers for the bits that are
 *          set in n.
 * https://oeis.org/A253315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = XOR of bit indices where bit is set in n
 * Range:     u8 n <= 4095, u16 n <= 4095, u32 n <= 4095, u64 n <= 4095,
 *            u128 n <= 4095, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t i = 0;
    idx_t m = n;

    while (m)
    {
        if (m & 1)
            res ^= (seq_t)i;
        m >>= 1;
        ++i;
    }

    return res;
}
