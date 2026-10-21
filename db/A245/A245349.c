/*
 * A245349  Sum of digits of n written in fractional base 7/4.
 * https://oeis.org/A245349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of digits of n written in fractional base 7/4
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t x = n;
    while (x > 0) {
        seq_t d = x % 7;              /* least‑significant digit */
        sum += d;
        x = (x / 7) * 4;              /* advance to the next digit (4/7 of the quotient) */
    }
    return sum;
}
