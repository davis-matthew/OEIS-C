/*
 * A300068  A sequence based on the period 6 sequence A300067.
 * https://oeis.org/A300068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*floor(n/6) + f(n mod 6), where f = [0, 0, 0, 1, 2, 2]
 * Range:     u8 n <= 512, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint8_t f[6] = {0, 0, 0, 1, 2, 2};

seq_t a(idx_t n)
{
    idx_t q = n / 6;          /* floor(n/6) */
    idx_t r = n % 6;          /* n mod 6 */
    return (seq_t)q * 3 + (seq_t)f[r];
}
