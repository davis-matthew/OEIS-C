/*
 * A319655  Write n in 7-ary, sort digits into increasing order.
 * https://oeis.org/A319655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = value of the base-7 digits of n arranged in * increasing
 *            order (most significant digits first)
 * Range:     u8 n <= 284, u16 n <= 2400, u32 n <= 2400, u64 n <= 2400,
 *            u128 n <= 2400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Provided by the build system.  seq_t is an unsigned integer type
 * (possibly up to 128 bits or arbitrary-precision).  idx_t is uint64_t. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Maximum digits for a 64-bit idx_t in base 7 is
     * less than 64, so a fixed array suffices. */
    uint8_t digits[64];
    idx_t tmp = n;
    size_t len = 0;

    /* Extract digits LSB first */
    while (tmp > 0) {
        digits[len++] = (uint8_t)(tmp % 7);
        tmp /= 7;
    }

    /* Reverse to MSB first */
    for (size_t i = 0; i < len / 2; ++i) {
        uint8_t t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    /* Sort ascending (MSB to LSB) – a simple bubble sort is fine as
     * the number of digits is very small. */
    for (size_t i = 0; i < len; ++i) {
        for (size_t j = 0; j + 1 < len - i; ++j) {
            if (digits[j] > digits[j + 1]) {
                uint8_t t = digits[j];
                digits[j] = digits[j + 1];
                digits[j + 1] = t;
            }
        }
    }

    /* Compute the number represented by the sorted digits in base 7 */
    seq_t result = 0;
    for (size_t i = 0; i < len; ++i) {
        result = result * 7 + digits[i];
    }

    return result;
}
