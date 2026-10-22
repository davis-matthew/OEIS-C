/*
 * A344658  a(n) = a^a - b^b + c^c - ... -+ d^d where the decimal expansion of
 *          n is abc...d.
 * https://oeis.org/A344658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = a^a - b^b + c^c - d^d + ... where a,b,c,d are the decimal
 *            digits * of n from most significant to least
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 39, i64 n <= 39, i128 n <= 39,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* Extract decimal digits of n into an array with most significant digit first. */
    const int MAX_DIGITS = 20;          /* enough for 64-bit integers (max 20 digits). */
    uint8_t digits[MAX_DIGITS];
    int len = 0;

    if (n == 0) {
        digits[len++] = 0;              /* 0 has one digit '0'. */
    } else {
        idx_t temp = n;
        while (temp > 0) {
            digits[len++] = temp % 10;
            temp /= 10;
        }
    }

    /* Compute alternating sum of digit^digit, starting with addition for the most
     * significant digit. 0^0 is defined as 1. */
    seq_t result = 0;
    bool add = true;
    for (int i = len - 1; i >= 0; --i) {
        uint8_t d = digits[i];
        seq_t val = (d == 0) ? 1 : big_pow((seq_t)d, (seq_t)d);
        result = add ? result + val : result - val;
        add = !add;
    }

    return result;
}
