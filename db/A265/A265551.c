/*
 * A265551  a(n) = smallest base-6 palindrome m >= n such that every base-6
 *          digit of n is <= the corresponding digit of m; m is written in
 *          base 6.
 * https://oeis.org/A265551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the base‑6 palindrome m ≥ n such that every base‑6 *
 *            digit of n is ≤ the corresponding digit of m. The function
 *            returns * the decimal integer whose digits are the base‑6 digits
 *            of m
 * Range:     u8 n <= 38, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* digits of n in base 6, least significant first */
    uint8_t d[128];          /* enough for numbers up to 2^128 */
    unsigned len = 0;
    idx_t tmp = n;

    if (tmp == 0) {
        d[0] = 0;
        len = 1;
    } else {
        while (tmp) {
            d[len++] = (uint8_t)(tmp % 6);
            tmp /= 6;
        }
    }

    /* force the number to be a palindrome, keeping each digit ≥ original */
    for (unsigned i = 0; i < len / 2; ++i) {
        unsigned j = len - 1 - i;
        uint8_t v = d[i] > d[j] ? d[i] : d[j];
        d[i] = d[j] = v;
    }

    /* build the decimal integer formed by those base‑6 digits */
    seq_t res = 0;
    for (int i = (int)len - 1; i >= 0; --i) {
        res = res * 10 + d[i];
    }

    return res;
}
