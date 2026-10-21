/*
 * A265550  a(n) = smallest base-6 palindrome m >= n such that every base-6
 *          digit of n is <= the corresponding digit of m; m is written in
 *          base 10.
 * https://oeis.org/A265550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest base‑6 palindrome m ≥ n such that every base‑6 *
 *            digit of n is ≤ the corresponding digit of m; m is * returned in
 *            base‑10
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t tmp = (seq_t)n;
    uint8_t rev[64];
    int len = 0;

    /* Extract digits of n in base 6 (least‑significant first). */
    while (tmp > 0) {
        rev[len++] = (uint8_t)(tmp % 6);
        tmp /= 6;
    }

    /* Build an array of the digits in most‑significant‑first order. */
    uint8_t msb[64];
    for (int i = 0; i < len; ++i)
        msb[i] = rev[len - 1 - i];

    /* Construct the smallest palindrome satisfying the digitwise
     * inequality constraints. */
    uint8_t pal[64];
    for (int i = 0; i < len / 2; ++i) {
        uint8_t v = msb[i] > msb[len - 1 - i] ? msb[i] : msb[len - 1 - i];
        pal[i] = pal[len - 1 - i] = v;
    }
    if (len % 2)
        pal[len / 2] = msb[len / 2];

    /* Convert the palindrome back to base‑10. */
    seq_t result = 0;
    for (int i = 0; i < len; ++i)
        result = result * 6 + pal[i];

    return result;
}
