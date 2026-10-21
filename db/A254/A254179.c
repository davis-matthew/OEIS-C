/*
 * A254179  Decimal expansion of atomic unit of time in s.
 * https://oeis.org/A254179
 *
 * Offset:    -16    (a(-16) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = digit_{−n−16} of 2418884326502, 0 outside that range
 * Range:     u8 n <= -7, u16 n <= -7, u32 n <= -7, u64 n <= -7, u128 n <= -7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    /* digits of 2418884326502, indices 0..12 correspond to n = -16..-4 */
    static const unsigned char digits[13] = {
        2,4,1,8,8,8,4,3,2,6,5,0,2
    };

    if (n < -16 || n > -4)
        return 0;
    return (seq_t)digits[n + 16];
}
