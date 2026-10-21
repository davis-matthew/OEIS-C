/*
 * A265547  a(n) = smallest base-4 palindrome m >= n such that every base-4
 *          digit of n is <= the corresponding digit of m; m is written in
 *          base 4.
 * https://oeis.org/A265547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest base‑4 palindrome m ≥ n such that for every
 *            position i, the base‑4 digit of n does not exceed the
 *            corresponding digit of m. The palindrome is returned as a
 *            decimal integer whose decimal digits are the base‑4 digits of
 *            the palindrome
 * Range:     u8 n <= 18, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   max_u8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/max_u8.h"
/* seq_t and idx_t are supplied by the build environment. */

seq_t a(idx_t n)
{
    /* Convert n to base‑4 digits (least significant first). */
    unsigned char d[128];
    size_t len = 0;

    if (n == 0) {
        d[0] = 0;
        len = 1;
    } else {
        idx_t t = n;
        while (t != 0) {
            d[len++] = t & 3;   /* t % 4  (since base 4) */
            t >>= 2;            /* t / 4  */
        }
    }

    /* Build palindrome digits of the same length; for each mirrored pair
       choose the larger digit to satisfy the constraint. */
    unsigned char p[128];
    for (size_t i = 0; i < len; ++i) {
        size_t j = len - 1 - i;
        unsigned char dig = max_u8(d[i], d[j]);
        p[i] = dig;
        p[j] = dig;
    }

    /* Convert digit array to a decimal integer (digits 0‑3). */
    seq_t res = 0;
    for (size_t i = len; i > 0; --i) {
        res = res * 10 + p[i - 1];
    }

    return res;
}
