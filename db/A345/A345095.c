/*
 * A345095  a(n) is the sum of the two preceding terms if n is even or n < 8,
 *          and the sum of the two rightmost digits of a(n-1) if n is odd,
 *          with a(0) = 0, a(1) = 1.
 * https://oeis.org/A345095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = a(n-1)+a(n-2) if n is even or n < 8; a(n) = sum of the
 *            two rightmost digits of a(n-1) if n is odd and n >= 8. a(0) = 0,
 *            a(1) = 1
 * Range:     u8 n <= 61, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    seq_t prev_prev = (seq_t)0; /* a(0) */
    seq_t prev      = (seq_t)1; /* a(1) */
    seq_t curr = 0;

    for (idx_t i = 2; i <= n; ++i) {
        if (((i & 1) == 0) || (i < 8)) {
            curr = prev + prev_prev;           /* sum of two preceding terms */
        } else {
            seq_t tens = (prev / 10) % 10;      /* two rightmost digits of a(i-1) */
            seq_t ones = prev % 10;
            curr = tens + ones;                /* sum of the two digits */
        }

        if (i == n)
            return curr;

        prev_prev = prev;
        prev      = curr;
    }

    return curr; /* unreachable, but keeps compiler happy */
}
