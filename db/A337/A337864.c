/*
 * A337864  a(n) is the number formed by removing from n each digit if it is a
 *          duplicate of the previous digit, from left to right.
 * https://oeis.org/A337864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number obtained by deleting any decimal digit that follows an
   identical digit (consecutive duplicates are removed). */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* store decimal digits of n in reverse order */
    uint8_t digits[21];          /* max 20 decimal digits for uint64_t */
    size_t len = 0;
    uint64_t tmp = n;
    while (tmp > 0) {
        digits[len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }

    /* rebuild number, skipping digits equal to the preceding kept digit */
    seq_t result = 0;
    uint8_t prev = 0xFF;     /* impossible initial value */
    size_t i = len;
    while (i--) {
        uint8_t d = digits[i];
        if (d != prev) {
            result = result * 10 + d;
            prev   = d;
        }
    }
    return result;
}
