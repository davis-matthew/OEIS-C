/*
 * A257601  a(n) = (n^4 + 20*n^3 + 125*n^2 + 250*n + 24)/12.
 * https://oeis.org/A257601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 + 20*n^3 + 125*n^2 + 250*n + 24)/12
 * Range:     u8 n <= 0, u16 n <= 11, u32 n <= 271, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;          /* n as seq_t */
    seq_t n2 = x * x;            /* n^2 */
    seq_t n3 = n2 * x;           /* n^3 */
    seq_t n4 = n3 * x;           /* n^4 */
    seq_t sum = n4
              + (seq_t)20 * n3
              + (seq_t)125 * n2
              + (seq_t)250 * x
              + (seq_t)24;
    return sum / (seq_t)12;
}
