/*
 * A259044  Period 8 sequence [ 0, 1, 0, 1, 1, 1, 0, 1, ...].
 * https://oeis.org/A259044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = periodic sequence of period 8: [0,1,0,1,1,1,0,1]
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t pattern[8] = { 0, 1, 0, 1, 1, 1, 0, 1 };
    return pattern[(size_t)(n & 7)];
}
