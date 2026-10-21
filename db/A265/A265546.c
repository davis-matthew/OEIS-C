/*
 * A265546  a(n) = smallest base-4 palindrome m >= n such that every base-4
 *          digit of n is <= the corresponding base-4 digit of m; m is written
 *          in base 10.
 * https://oeis.org/A265546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest base-4 palindrome m ≥ n such that each base‑4
 *            digit of n is ≤ the corresponding digit of m. Algorithm: extract
 *            the base‑4 digits of n, take the digitwise maximum of each
 *            mirrored pair to obtain the minimal palindrome, then convert
 *            back to base‑10
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Extract base‑4 digits of n into array digits[0] = least significant */
    seq_t tmp = (seq_t)n;
    uint8_t digits[128];
    size_t len = 0;

    if (tmp == (seq_t)0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (tmp > (seq_t)0) {
            digits[len++] = (uint8_t)(tmp % (seq_t)4);
            tmp /= (seq_t)4;
        }
    }

    /* Build the palindrome by taking max of mirrored digits */
    uint8_t pal[128];
    for (size_t i = 0; i < len; ++i) {
        uint8_t d1 = digits[i];
        uint8_t d2 = digits[len - 1 - i];
        pal[i] = (d1 > d2) ? d1 : d2;  /* digitwise maximum */
    }

    /* Convert pal back to base‑10 */
    seq_t result = (seq_t)0;
    for (size_t i = len; i > 0; --i) {
        result = result * (seq_t)4 + (seq_t)pal[i - 1];
    }

    return result;
}
