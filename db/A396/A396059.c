/*
 * A396059  a(n) is the action of "Rule 90" elementary cellular automaton on
 *          the binary representation of n if the cells may only expand into
 *          the significant bit.
 * https://oeis.org/A396059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (2 * n) XOR floor(n / 2)
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return (sn << 1) ^ (sn >> 1);
}
