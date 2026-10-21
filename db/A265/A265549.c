/*
 * A265549  a(n) = smallest base-5 palindrome m >= n such that every base-5
 *          digit of n is <= the corresponding digit of m; m is written in
 *          base 5.
 * https://oeis.org/A265549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest base‑5 palindrome m >= n such that every base‑5
 *            digit of n is <= the corresponding digit of m; return m written
 *            as a base‑10 integer composed of the base‑5 digits
 * Range:     u8 n <= 27, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Extract base‑5 digits of n (least significant first). */
    uint8_t d[64];
    size_t len = 0;
    idx_t temp = n;
    if (temp == 0) {
        d[0] = 0;
        len = 1;
    } else {
        while (temp > 0) {
            d[len++] = (uint8_t)(temp % 5);
            temp /= 5;
        }
    }

    /* Construct palindrome digits that dominate n. */
    uint8_t p[64];
    for (size_t i = 0; i < len; ++i) p[i] = d[i];
    for (size_t i = 0; i < (len + 1) / 2; ++i) {
        size_t j = len - 1 - i;
        uint8_t mx = (d[i] > d[j]) ? d[i] : d[j];
        p[i] = mx;
        p[j] = mx;
    }

    /* Convert palindrome digits back to decimal (as base‑10 digits). */
    seq_t result = 0;
    seq_t pow10   = 1;
    for (size_t i = 0; i < len; ++i) {
        result += (seq_t)p[i] * pow10;
        pow10   = pow10 * (seq_t)10;
    }

    return result;
}
