/*
 * A270740  Period 9: repeat 0,1,2,2,0,1,1,2,0.
 * https://oeis.org/A270740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - floor(n/3)) mod 3
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* equivalent to (n - n/3) % 3 */
    idx_t v = n - n / 3;
    return (seq_t)(v % 3);
}
