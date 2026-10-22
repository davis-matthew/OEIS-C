/*
 * A324027  One of the two successive approximations up to 5^n for the 5-adic
 *          integer sqrt(-6). This is the 2 (mod 5) case (except for n = 0).
 * https://oeis.org/A324027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = unique x ≡ 2 (mod 5) such that x² ≡ –6 (mod 5ⁿ) (a(0)=0).
 *            The routine builds the 5‑adic digits of that root one at a time
 *            by Hensel lifting
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0u;

    /* Number of 5‑adic digits needed. */
    size_t len = (size_t)n;

    /* Allocate arrays for the base‑5 digits (least‑significant first)
       and for the convolution of digit products. */
    uint8_t *digits = (uint8_t *)malloc(len * sizeof(uint8_t));
    if (!digits) return (seq_t)0u;

    int32_t *conv = (int32_t *)calloc(2 * len + 1, sizeof(int32_t));
    if (!conv) { free(digits); return (seq_t)0u; }

    /* Initial digit (2) and its contribution to the convolution. */
    digits[0] = 2;
    conv[0] = 4;            /* 2 * 2 */

    /* Iteratively add one digit at a time. */
    for (size_t pos = 1; pos < len; ++pos)
    {
        /* Compute the digit t at position pos of (a² + 6) / 5^pos. */
        int carry = 0, t = 0;
        for (size_t i = 0; i <= pos; ++i)
        {
            int temp = conv[i];
            if (i == 0) temp += 6;    /* add 6 at the 5⁰ position */
            temp += carry;
            int digit = temp % 5;
            carry = temp / 5;
            if (i == pos) t = digit;
        }

        /* Hensel lift: next digit k satisfying (d + 4k) ≡ 0 (mod 5). */
        int k = (5 - ((t * 4) % 5)) % 5;
        digits[pos] = (uint8_t)k;

        /* Update the convolution array with the new digit k. */
        for (size_t off = 0; off < pos; ++off)
            conv[off + pos] += 2 * k * digits[off];
        conv[2 * pos] += k * k;
    }

    /* Convert the base‑5 digits back to a numeric value. */
    seq_t result = 0;
    for (size_t i = len; i-- > 0; )
        result = result * ((seq_t)5) + digits[i];

    free(digits);
    free(conv);
    return result;
}
