/*
 * A265552  a(n) = smallest base-7 palindrome m >= n such that every base-7
 *          digit of n is <= the corresponding digit of m; m is written in
 *          base 10.
 * https://oeis.org/A265552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest base-7 palindrome m ≥ n so that every base-7
 *            digit of n is ≤ the corresponding digit of m. The result is
 *            returned in base 10
 * Range:     u8 n <= 54, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Extract the base‑7 digits of n (least significant first). */
    uint8_t digits[64];
    size_t len = 0;
    idx_t tmp = n;

    if (tmp == 0) {
        /* n is 0, treat as one digit 0. */
        len = 1;
        digits[0] = 0;
    } else {
        while (tmp > 0) {
            /* Since tmp < UINT64_MAX, %7 and /7 stay within idx_t. */
            digits[len++] = tmp % 7;
            tmp /= 7;
        }
    }

    /* Make the digit array a palindrome with minimal possible digits,
       respecting that each position must be ≥ the corresponding digit of n. */
    for (size_t i = 0; i * 2 + 1 < len; ++i) {
        size_t j = len - 1 - i;
        uint8_t need = digits[i] > digits[j] ? digits[i] : digits[j];
        digits[i] = need;
        digits[j] = need;
    }

    /* Convert the palindrome digits back to a base-10 number. */
    seq_t m = 0;
    seq_t pow = 1;          /* 7^i as seq_t. */
    for (size_t i = 0; i < len; ++i) {
        m += digits[i] * pow;
        pow *= 7;
    }

    return m;
}
