/*
 * A256304  Apply the transformation 0 -> 1 -> 2 -> 3 -> 0 to the digits of n
 *          written in base 4; do not convert back to base 10.
 * https://oeis.org/A256304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = transform each base-4 digit of n by +1 modulo 4, then
 *            read the resulting digit string as a decimal integer
 * Range:     u8 n <= 31, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case: 0 maps to "1" (since 0 -> 1 in base-4). */
    if (n == 0)
        return (seq_t)1;

    /* Extract base-4 digits of n. */
    uint16_t digits[64];  /* 64 digits are more than enough for 64-bit n. */
    int pos = 0;
    idx_t tmp = n;
    while (tmp != 0) {
        digits[pos++] = (uint16_t)(tmp & 3);   /* tmp % 4 */
        tmp >>= 2;                            /* tmp / 4 */
    }

    /* Build the answer by processing digits from most significant to least. */
    seq_t result = 0;
    for (int i = pos - 1; i >= 0; --i) {
        uint8_t d = digits[i] & 3;
        uint8_t t = (d + 1) & 3;               /* +1 modulo 4 */
        result = result * 10 + t;
    }
    return result;
}
