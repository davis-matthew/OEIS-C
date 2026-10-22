/*
 * A319726  Write n in 9-ary, sort digits into decreasing order.
 * https://oeis.org/A319726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   a(n) = decimal value of the base-9 number obtained from n’s
 *            base‑9 digits sorted in decreasing order
 * Range:     u8 n <= 84, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n) {
    /* extract digits of n in base 9 (least significant first) */
    uint8_t digits[64];
    int len = 0;
    idx_t tmp = n;
    if (tmp == 0) {
        digits[len++] = 0;
    } else {
        while (tmp > 0) {
            digits[len++] = (uint8_t)(tmp % 9);
            tmp /= 9;
        }
    }
    /* count digits for sorting */
    uint8_t count[9] = {0};
    for (int i = 0; i < len; ++i) {
        ++count[digits[i]];
    }
    /* rebuild digits in decreasing order */
    int pos = 0;
    for (int d = 8; d >= 0; --d) {
        while (count[d]--) {
            digits[pos++] = (uint8_t)d;
        }
    }
    /* convert sorted digits back to a decimal value */
    seq_t result = 0;
    for (int i = 0; i < len; ++i) {
        result = result * 9 + (seq_t)digits[i];
    }
    return result;
}
