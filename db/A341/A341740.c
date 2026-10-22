/*
 * A341740  a(n) is the maximum value of the magic constant in a normal magic
 *          triangle of order n.
 * https://oeis.org/A341740
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^2 + n - 6)/2 (valid for n > 2)
 * Range:     u8 n <= 12, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;          /* undefined for n < 3, return 0 */
    uint64_t nn = (uint64_t)n;
    uint64_t val = (3 * nn * nn + nn - 6) / 2;
    return (seq_t)val;
}
