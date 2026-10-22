/*
 * A382401  a(n) is the number formed by removing all copies of the smallest
 *          digit of n, or 0 if no digits remain.
 * https://oeis.org/A382401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = the decimal number obtained by removing all copies of the
 *            smallest digit of the decimal representation of n; 0 if no
 *            digits remain
 * Range:     u8 n <= 1255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Extract digits of n into an array, least significant first. */
    idx_t tmp = n;
    /* maximum number of decimal digits for uint64_t is 20. */
    unsigned char digits[20];
    int len = 0;
    while (tmp > 0) {
        digits[len++] = tmp % 10;
        tmp /= 10;
    }

    /* Find the smallest digit. */
    unsigned char min_digit = 9;
    for (int i = 0; i < len; ++i) {
        if (digits[i] < min_digit)
            min_digit = digits[i];
    }

    /* Build the result by keeping digits that are not the smallest. */
    seq_t result = 0;
    bool any = false;
    for (int i = len - 1; i >= 0; --i) {
        unsigned char d = digits[i];
        if (d != min_digit) {
            result = result * 10 + d;
            any = true;
        }
    }

    if (!any)
        return 0;
    return result;
}
