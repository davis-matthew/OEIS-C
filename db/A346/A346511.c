/*
 * A346511  a(n) = bitwise XOR of decimal digits of n.
 * https://oeis.org/A346511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = bitwise XOR of decimal digits of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t t = n;
    seq_t res = 0;
    while (t != 0) {
        res ^= (seq_t)(t % 10);
        t /= 10;
    }
    return res;
}
