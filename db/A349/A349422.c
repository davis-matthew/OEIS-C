/*
 * A349422  Replace each decimal digit d of n with the digit that is d steps
 *          to the left of d. Interpret the digits of n as a cycle: one step
 *          to the left from the first digit is considered to be the last.
 * https://oeis.org/A349422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Compute A349422: for each decimal digit d of n, replace it by the digit that is
 * d steps to the left in the circular order of digits of n (with wrap‑around).
 */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Extract decimal digits of n into digits[0..len-1] (most significant first) */
    uint8_t digits[20];
    size_t len = 0;

    if (n == 0) {
        len = 1;
        digits[0] = 0;
    } else {
        idx_t tmp = n;
        /* First collect digits in reverse order */
        while (tmp > 0) {
            digits[len++] = tmp % 10;
            tmp /= 10;
        }
        /* Reverse to obtain normal order */
        for (size_t i = 0; i < len / 2; ++i) {
            uint8_t t = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = t;
        }
    }

    /* Compute the transformed digits */
    uint8_t newdigits[20];
    for (size_t i = 0; i < len; ++i) {
        uint8_t d = digits[i];
        size_t target = (len + i - (d % len)) % len; /* d steps left, wrapped */
        newdigits[i] = digits[target];
    }

    /* Build the resulting integer */
    seq_t result = 0;
    for (size_t i = 0; i < len; ++i) {
        result = result * 10 + newdigits[i];
    }
    return result;
}
