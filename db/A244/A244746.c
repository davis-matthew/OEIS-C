/*
 * A244746  Numbers obtained by concatenating the squares of the digits of
 *          Catalan(n).
 * https://oeis.org/A244746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A048385(A000108(n)) // Catalan(n) → concat of squared
 *            digits
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   catalan, digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/catalan.h"
#include "../utils/digit_len.h"

static const unsigned int digit_sq[10] = {
    0, 1, 4, 9, 16, 25, 36, 49, 64, 81
};

seq_t a(idx_t n)
{
    seq_t cat = catalan(n);

    /* Number of decimal digits in cat */
    size_t len = digit_len(cat);
    if (len == 0) len = 1;                     /* cat == 0 */

    /* Allocate buffer for digits (least‑significant first) */
    uint8_t *digits = (uint8_t*)malloc(len);
    if (!digits) return (seq_t)0;                /* allocation failed */

    /* Extract digits */
    seq_t t = cat;
    for (size_t i = 0; i < len; ++i) {
        digits[i] = (uint8_t)(t % (seq_t)10);
        t /= (seq_t)10;
    }

    /* Build concatenated square number from most‑significant digit */
    seq_t result = 0;
    for (size_t i = len; i-- > 0;) {
        unsigned int s = digit_sq[digits[i]];
        seq_t mult = (s < 10) ? (seq_t)10 : (seq_t)100;
        result = result * mult + (seq_t)s;
    }

    free(digits);
    return result;
}
