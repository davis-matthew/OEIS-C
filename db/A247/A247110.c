/*
 * A247110  n + reversal of digits of n, when n is not palindromic.
 * https://oeis.org/A247110
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 11 * (2 * firstDigit(n) + lastDigit(n) - adjustment)
 *            where adjustment = 1 if lastDigit(n) == 0, 9 if firstDigit(n) +
 *            lastDigit(n) > 9, 0 otherwise
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For idx_t >= 10 as per OEIS offset. */
    unsigned last = (unsigned)(n % 10);

    /* Find the most significant digit. */
    idx_t temp = n;
    while (temp >= 10)
        temp /= 10;
    unsigned first = (unsigned)temp;

    unsigned k = 2 * first + last;

    if (last == 0) {
        k -= 1;               /* case where n ends with 0 */
    } else if (first + last > 9) {
        k -= 9;               /* case where digit sum exceeds 9 */
    }

    return (seq_t)k * (seq_t)11;
}
