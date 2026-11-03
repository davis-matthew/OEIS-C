/*
 * A265567  Smallest base-6 palindrome m >= n, written in base 6.
 * https://oeis.org/A265567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest base‑6 palindrome m >= n, returned as the
 *            decimal number whose decimal digits equal the base‑6 digits of m
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 71 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Convert x to base‑6 digits (least significant first).
   The digits are stored in buf[0..len-1], each between 0 and 5.
   Return true when the digit string is a palindrome.
   buf is assumed large enough (>=64).       */
static bool base6_palindrome_and_digits(idx_t x, uint8_t *buf, size_t *len)
{
    size_t t = 0;
    if (x == 0) {
        buf[0] = 0;
        t = 1;
    } else {
        while (x > 0 && t < sizeof(buf)) {
            buf[t++] = (uint8_t)(x % 6);
            x /= 6;
        }
    }

    bool pal = true;
    for (size_t i = 0, j = t - 1; i < j; ++i, --j) {
        if (buf[i] != buf[j]) {
            pal = false;
            break;
        }
    }
    *len = t;
    return pal;
}

seq_t a(idx_t n)
{
    static uint8_t digits[64]; /* enough for any 64‑bit integer (~13 digits base‑6) */
    size_t len;

    for (idx_t m = n; ; ++m) {
        if (base6_palindrome_and_digits(m, digits, &len)) {
            /* Convert the base‑6 digits (stored least significant first) to
               a decimal number that has the same decimal digits. */
            seq_t value = 0;
            for (size_t i = 0; i < len; ++i) {
                /* process most significant digit first */
                value = value * 10 + (seq_t)digits[len - 1 - i];
            }
            return value;
        }
    }
}
