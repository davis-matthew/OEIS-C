/*
 * A256306  Apply the transformation 0 -> 1 -> 2 -> 3 -> 4 -> 5 -> 0 to the
 *          digits of n written in base 6; do not convert back to base 10.
 * https://oeis.org/A256306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = shift digits of n in base 6 by +1 mod 6, interpreted as a
 *            decimal integer (base 10)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base‑6 representation of n, least‑significant digit first */
    unsigned digits[32];
    size_t len = 0;

    if (n == 0) {
        /* "0" → shift → "1" */
        return (seq_t)1;
    }

    while (n > 0) {
        digits[len++] = (unsigned)(n % 6);
        n /= 6;
    }

    seq_t res = 0;
    for (size_t i = 0; i < len; ++i) {
        unsigned d = digits[len - 1 - i]; /* most‑significant digit first */
        d = (d + 1) % 6;                  /* shift digit */
        res = res * 10 + (seq_t)d;        /* build decimal integer */
    }

    return res;
}
