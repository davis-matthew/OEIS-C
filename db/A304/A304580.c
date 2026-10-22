/*
 * A304580  Period 16: repeat 1,8,3,6,5,4,7,2,9,0,7,2,5,4,3,6.
 * https://oeis.org/A304580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1,8,3,6,5,4,7,2,9,0,7,2,5,4,3,6 repeated with period 16
 *            (offset 1)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t pattern[16] = {
        1, 8, 3, 6, 5, 4, 7, 2,
        9, 0, 7, 2, 5, 4, 3, 6
    };
    size_t idx = (size_t)((n - 1) % 16);
    return pattern[idx];
}
