/*
 * A279321  Period 7: repeat [1, 3, 5, 7, 5, 3, 1].
 * https://oeis.org/A279321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1,3,5,7,5,3,1 repeating with period 7 (a(0)=1)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    static const seq_t pattern[7] = {
        (seq_t)1, (seq_t)3, (seq_t)5, (seq_t)7,
        (seq_t)5, (seq_t)3, (seq_t)1
    };
    return pattern[(size_t)(n % 7)];
}
