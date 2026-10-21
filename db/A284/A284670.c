/*
 * A284670  Exponents of powers of 5 that contain all ten decimal digits.
 * https://oeis.org/A284670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n + 40 for n > 25 (conjecture); this implementation
 *            searches for the exact exponents of powers of 5 that contain all
 *            ten decimal digits
 * Range:     u8 n <= 215, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state */
    static seq_t *exponents = NULL;
    static size_t exponents_len = 0, exponents_cap = 0;

    static uint8_t *digits = NULL;    /* decimal digits of 5^current_exp (least‑significant first) */
    static size_t digits_len = 0, digits_cap = 0;
    static idx_t current_exp = 0;     /* exponent of the current power of 5 */

    /* initialise lazy */
    if (exponents == NULL) {
        /* initial power 5^0 = 1 */
        digits_cap = 64;
        digits = (uint8_t *)malloc(digits_cap);
        digits[0] = 1;
        digits_len = 1;
    }

    /* generate terms until we have at least n entries */
    while (exponents_len < (size_t)n) {
        /* Increase the exponent and multiply the current power by 5 */
        current_exp++;
        /* ensure space for potential new digits */
        if (digits_len + 4 > digits_cap) {   /* +4 gives headroom for carry */
            size_t new_cap = digits_cap ? digits_cap * 2 : 64;
            while (new_cap < digits_len + 4)
                new_cap <<= 1;
            digits = (uint8_t *)realloc(digits, new_cap);
            digits_cap = new_cap;
        }
        /* multiply by 5 */
        uint32_t carry = 0;
        for (size_t i = 0; i < digits_len; ++i) {
            uint32_t tmp = (uint32_t)digits[i] * 5 + carry;
            digits[i] = (uint8_t)(tmp % 10);
            carry = tmp / 10;
        }
        while (carry) {
            if (digits_len >= digits_cap) {
                size_t new_cap = digits_cap * 2;
                digits = (uint8_t *)realloc(digits, new_cap);
                digits_cap = new_cap;
            }
            digits[digits_len++] = (uint8_t)(carry % 10);
            carry /= 10;
        }

        /* check if all digits 0–9 appear */
        unsigned mask = 0;
        for (size_t i = 0; i < digits_len; ++i) {
            mask |= 1U << digits[i];
            if (mask == 0x3FF)   /* all ten bits set */
                break;
        }
        if (mask == 0x3FF) {
            /* store the exponent */
            if (exponents_len >= exponents_cap) {
                size_t new_cap = exponents_cap ? exponents_cap * 2 : 64;
                exponents = (seq_t *)realloc(exponents, new_cap * sizeof(seq_t));
                exponents_cap = new_cap;
            }
            exponents[exponents_len++] = (seq_t)current_exp;
        }
    }

    return exponents[n - 1];
}
