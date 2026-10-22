/*
 * A328375  Numbers k such that the decimal expansion of 2^k contains the
 *          substring 777.
 * https://oeis.org/A328375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that the decimal expansion of 2^k contains
 * the substring "777".  The function searches powers of two incrementally,
 * caching results in a static array.  This method works for all provided
 * builds, including those where seq_t is an arbitrary‑precision integer. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
/* seq_t and idx_t are supplied by the build environment. */

seq_t a(idx_t n)
{
    /* Cache of found exponents */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;

    /* Big‐integer representation of 2^k in decimal (least significant digit first). */
    static uint8_t *digits = NULL;
    static size_t digits_len = 0;   /* number of used digits */
    static size_t digits_cap = 0;   /* allocated capacity */
    static uint64_t cur_k = 0;      /* current exponent */

    /* One‑time initialization */
    if (terms == NULL) {
        terms_cap   = 16;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        terms_len  = 0;

        digits_cap  = 64;
        digits = (uint8_t *)malloc(digits_cap);
        digits_len = 1;            /* 2^0 = 1 */
        digits[0]  = 1;
        cur_k = 0;
    }

    /* Return cached result if available */
    if (n <= terms_len)
        return terms[n - 1];

    /* Extend cache until we have at least n terms */
    while (terms_len < n) {
        /* Advance to the next power of two: 2^(cur_k+1) */
        ++cur_k;
        uint8_t carry = 0;
        for (size_t i = 0; i < digits_len; ++i) {
            uint16_t tmp = (uint16_t)digits[i] * 2 + carry;
            digits[i] = (uint8_t)(tmp % 10);
            carry = (uint8_t)(tmp / 10);
        }
        if (carry) {               /* one more digit is added */
            if (digits_len == digits_cap) {
                digits_cap = digits_cap * 2;
                digits = (uint8_t *)realloc(digits, digits_cap);
            }
            digits[digits_len++] = carry;
        }

        /* Check whether the decimal expansion contains "777" */
        bool found = false;
        if (digits_len >= 3) {
            for (size_t msb = digits_len - 1; msb >= 2; --msb) {
                if (digits[msb]   == 7 &&
                    digits[msb-1] == 7 &&
                    digits[msb-2] == 7) {
                    found = true;
                    break;
                }
            }
        }

        /* Store the exponent if found */
        if (found) {
            if (terms_len == terms_cap) {
                terms_cap = terms_cap * 2;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)cur_k;
        }
    }

    return terms[n - 1];
}
