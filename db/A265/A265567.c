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
 * Helpers:   base6_palindrome_and_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/base6_palindrome_and_digits.h"

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
