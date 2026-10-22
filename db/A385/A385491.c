/*
 * A385491  Period 8: repeat [1, 8, 4, 3, 6, 5, 7, 2].
 * https://oeis.org/A385491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [1,8,4,3,6,5,7,2][n mod 8]
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    static const seq_t cycle[8] = {
        (seq_t)1, (seq_t)8, (seq_t)4, (seq_t)3,
        (seq_t)6, (seq_t)5, (seq_t)7, (seq_t)2
    };
    return cycle[(size_t)(n & 7)];
}
