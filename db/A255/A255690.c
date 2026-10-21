/*
 * A255690  Convert n to base 5, move the most significant digit to the least
 *          significant one and convert back to base 10.
 * https://oeis.org/A255690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   rotate base-5 digits of n left by one position
 * Range:     u8 n <= 175, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Extract base-5 digits, least significant first */
    uint8_t digits[64];
    idx_t tmp = n;
    int len = 0;
    while (tmp)
    {
        digits[len++] = tmp % 5;
        tmp /= 5;
    }

    /* Most significant digit becomes the new least significant one */
    seq_t msd = digits[len - 1];
    seq_t result = msd;
    seq_t power = 5;

    for (int i = 0; i < len - 1; ++i)
    {
        result += (seq_t)digits[i] * power;
        power *= 5;
    }

    return result;
}
