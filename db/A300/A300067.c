/*
 * A300067  Period 6: repeat [0, 0, 0, 1, 2, 2].
 * https://oeis.org/A300067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n mod 6)/3) + floor((n mod 6)/4)
 * Range:     u8 n <= 113, u16 n <= 113, u32 n <= 113, u64 n <= 113,
 *            u128 n <= 113, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use the periodic nature: n mod 6 determines the value. */
    idx_t m = n % 6;
    /* floor division by constants fits in idx_t, then cast to seq_t. */
    return (seq_t)(m / 3) + (seq_t)(m / 4);
}
