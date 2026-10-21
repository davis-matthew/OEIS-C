/*
 * A269445  a(n) = Sum_{k=0..n} floor(k/13).
 * https://oeis.org/A269445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} floor(k/13)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t q = (seq_t)(n / 13);
    seq_t r = (seq_t)(n % 13);
    seq_t part1 = 13 * (q * (q - 1) / 2);
    seq_t part2 = q * (r + 1);
    return part1 + part2;
}
