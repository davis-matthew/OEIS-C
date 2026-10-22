/*
 * A305493  A binary encoding of the decimal representation of a number: for
 *          any number n >= 0, consider its decimal representation and replace
 *          each 9 with "111111111" and each other digit d with a "0" followed
 *          by d "1"s and interpret the result as a binary string.
 * https://oeis.org/A305493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = binary-encoded decimal representation of n (replace each
 *            9 by 111111111 and each other digit d by 0 followed by d 1's)
 * Range:     u8 n <= 8, u16 n <= 87, u32 n <= 6887, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Gather decimal digits of n (least significant first). */
    uint8_t digits[20];
    idx_t len = 0;
    idx_t tmp = n;
    do {
        digits[len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    } while (tmp != 0);

    /* Convert the digits to the encoded binary value. */
    seq_t val = 0;
    for (idx_t i = len; i > 0; --i) {
        unsigned d = digits[i - 1];
        if (d == 9) {
            val <<= 9;
            val |= (seq_t)0x1FFu;        /* nine 1's */
        } else {
            val <<= (d + 1);             /* leading 0 plus d 1's */
            if (d != 0) {
                /* (1 << d) - 1  yields d ones. */
                val |= (((seq_t)1 << d) - 1);
            }
        }
    }
    return val;
}
