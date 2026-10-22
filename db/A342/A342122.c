/*
 * A342122  a(n) is the remainder when the binary reverse of n is divided by
 *          n.
 * https://oeis.org/A342122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   a(n) = reverse_bits(n) mod n
 * Range:     u8 n <= 288, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t rev = 0;
    idx_t tmp = n;
    while (tmp) {
        rev = (rev << 1) | (seq_t)(tmp & 1);
        tmp >>= 1;
    }
    return rev % (seq_t)n;
}
