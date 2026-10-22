/*
 * A392049  a(n) is the numerator of the harmonic mean of the digits of n.
 * https://oeis.org/A392049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, frac
 * Formula:   a(n) = numerator of the harmonic mean of the decimal digits of
 *            n. If any digit is zero, the result is 0
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* Handle n=0: contains digit 0 */
    if (n == 0)
        return 0;

    /* Extract decimal digits, count them and store in an array. */
    unsigned char digits[20];            /* maximum 20 digits for 64-bit n */
    idx_t k = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        unsigned int d = tmp % 10;
        if (d == 0)          /* zero digit => numerator 0 */
            return 0;
        digits[k++] = (unsigned char)d;
        tmp /= 10;
    }

    /* Compute product of digits. */
    seq_t prod = 1;
    for (idx_t i = 0; i < k; ++i)
        prod *= (seq_t)digits[i];

    /* Compute sum of product / digit for each digit. */
    seq_t sum = 0;
    for (idx_t i = 0; i < k; ++i)
        sum += prod / (seq_t)digits[i];

    /* Numerator before reduction: k * prod. */
    seq_t numer = (seq_t)k * prod;

    /* Reduce fraction to lowest terms and return the numerator. */
    seq_t g = gcd(numer, sum);   /* gcd for seq_t is provided in utils */
    return numer / g;
}
