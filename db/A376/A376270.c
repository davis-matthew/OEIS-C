/*
 * A376270  a(n) is the product of the leading digit of n and the sum of the
 *          squares of its digits.
 * https://oeis.org/A376270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = leading_digit(n) * sum_of_squares_of_digits(n)
 * Range:     u8 n <= 6, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t temp = n;

    /* Sum of squares of digits */
    while (temp > 0) {
        unsigned d = temp % 10;          /* d fits in unsigned 0..9 */
        sum += (seq_t)d * (seq_t)d;      /* d^2 fits in seq_t */
        temp /= 10;
    }

    /* Leading digit */
    seq_t lead = 0;
    temp = n;
    if (temp != 0) {
        while (temp >= 10) temp /= 10;
        lead = temp;                     /* 0 <= lead <= 9 */
    }

    return lead * sum;
}
