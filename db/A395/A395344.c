/*
 * A395344  a(n) is the integer obtained by inserting the sum (modulo 10) of
 *          adjacent digits of n between them.
 * https://oeis.org/A395344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = insert the digit (d_i + d_{i+1}) mod 10 between each
 *            adjacent pair of decimal digits d_i of n
 * Range:     u8 n <= 23, u16 n <= 599, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Handle single–digit numbers directly */
    if (n <= 9)
        return (seq_t)n;

    /* Extract decimal digits of n (most significant first) */
    uint8_t digits[25];   /* Enough for 64‑bit numbers (<21 digits) */
    int len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }

    /* Reverse to get most significant digit first */
    for (int i = 0; i < len / 2; ++i) {
        uint8_t t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    /* Build the result by inserting sums between adjacent digits */
    seq_t result = digits[0];
    for (int j = 1; j < len; ++j) {
        unsigned sum = (unsigned)(digits[j - 1] + digits[j]) % 10;
        result = result * 10 + sum;
        result = result * 10 + digits[j];
    }

    return result;
}
