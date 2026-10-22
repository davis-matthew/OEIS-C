/*
 * A371092  a(n) = floor((A000265(3*n+1)+5)/6), where A000265 gives the odd
 *          part of its argument.
 * https://oeis.org/A371092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((odd_part(3*n+1)+5)/6)
 * Range:     u8 n <= 84, u16 n <= 9864, u32 n <= 9864, u64 n <= 9864,
 *            u128 n <= 9864, big any n
 * Helpers:   _odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_odd_part.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n * 3 + 1;   /* compute 3*n+1 in seq_t */
    x = _odd_part(x);             /* odd part of 3*n+1 */
    return (x + 5) / 6;           /* floor division */
}
