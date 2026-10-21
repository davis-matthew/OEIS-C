/*
 * A241494  Pyramid Top Numbers: write the decimal digits of 'n' (a
 *          nonnegative integer) and take successive absolute differences
 *          ("pyramidalization"). The number at the top of the pyramid is
 *          'a(n)'.
 * https://oeis.org/A241494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy, look
 * Formula:   a(n) = successive absolute differences of decimal digits until
 *            one digit remains
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Numbers with fewer than two digits are fixed points of the operation. */
    if (n < 10u)
        return (seq_t)n;

    /* Extract base‑10 digits of n into an array.  The maximum number of digits
       for a 64‑bit unsigned integer is 20, which fits comfortably in a
       fixed-size buffer. */
    uint8_t digits[20];
    size_t  len = 0;
    idx_t   tmp = n;

    while (tmp > 0u) {
        digits[len++] = (uint8_t)(tmp % 10u);
        tmp /= 10u;
    }

    /* The loop above stores the digits in reverse order.  Reverse them to
       obtain the natural left‑to‑right order. */
    for (size_t i = 0; i < len / 2; ++i) {
        uint8_t t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    /* Iterate the “pyramidalization” process until a single digit remains. */
    size_t cur = len;
    while (cur > 1) {
        for (size_t i = 0; i < cur - 1; ++i) {
            uint8_t a = digits[i];
            uint8_t b = digits[i + 1];
            digits[i] = (a > b) ? (a - b) : (b - a);
        }
        --cur;
    }

    return (seq_t)digits[0];
}
