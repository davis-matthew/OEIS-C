/*
 * A297330  Total variation of base-10 digits of n; see Comments.
 * https://oeis.org/A297330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum over adjacent decimal digits of the absolute
 *            difference of each pair. This is the total variation of the
 *            base-10 digits of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* For n < 10 there are no adjacent digit pairs */
    if (n < 10)
        return 0;

    /* Digits are at most 20 for 64-bit n, plus a small spare slot. */
    unsigned char digits[21];
    size_t pos = 0;

    /* Extract digits from least significant to most significant. */
    idx_t temp = n;
    while (temp > 0) {
        digits[pos++] = (unsigned char)(temp % 10);
        temp /= 10;
    }

    /* Compute the sum of absolute differences between adjacent
       digits in the natural left‑to‑right order. */
    seq_t sum = 0;
    for (size_t i = 1; i < pos; ++i) {
        unsigned char d1 = digits[pos - i];
        unsigned char d2 = digits[pos - i - 1];
        unsigned char diff = d1 > d2 ? d1 - d2 : d2 - d1;
        sum += diff;
    }

    return sum;
}
