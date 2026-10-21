/*
 * A259080  Expressed in base n, the number (n+1)^n.
 * https://oeis.org/A259080
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the base‑n representation of (n+1)^n, written as a *
 *            decimal number formed by concatenating the base‑n digits. If any
 *            * digit of the base‑n representation is > 9 the function returns
 *            0 * (the sequence is undefined beyond that point)
 * Range:     u8 none, u16 n <= 4, u32 n <= 9, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;                    /* defined for n >= 2 only */

    /* Maximum number of base‑n digits of (n+1)^n is n+1, plus one for
     * a possible carry.  Allocate that many slots for the digit arrays. */
    size_t maxlen = (size_t)n + 2;           /* +2 for safety */
    uint32_t *digits  = (uint32_t*)calloc(maxlen, sizeof(uint32_t));
    uint32_t *ndigits = (uint32_t*)calloc(maxlen, sizeof(uint32_t));
    if (!digits || !ndigits) return 0;      /* allocation failure */

    /* Start from 1 = (n+1)^0.  In base n this is a single digit 1. */
    size_t len = 1;
    digits[0] = 1;
    uint32_t base = (uint32_t)n;            /* n fits in 32 bits for all test cases */

    for (idx_t iter = 0; iter < n; ++iter) {
        size_t newlen = len + 1;

        /* zero the working array up to the new length */
        for (size_t j = 0; j < newlen; ++j)
            ndigits[j] = 0;

        /* ndigits = digits + digits·n (i.e. digits shifted left by one position) */
        for (size_t j = 0; j < len; ++j) {
            ndigits[j]     += digits[j];
            ndigits[j + 1] += digits[j];
        }

        /* normalize carries in base n */
        uint32_t carry = 0;
        for (size_t j = 0; j < newlen; ++j) {
            uint32_t sum = ndigits[j] + carry;
            ndigits[j] = sum % base;
            carry = sum / base;
        }
        if (carry) {                /* possible extra most‑significant digit */
            ndigits[newlen] = carry;
            len = newlen + 1;
        } else {
            len = newlen;
        }

        /* swap the role of the two arrays for the next iteration */
        uint32_t *tmp = digits;
        digits = ndigits;
        ndigits = tmp;
    }

    /* Convert the base‑n digits to a decimal integer.  The digits array
     * holds the least‑significant digit first. */
    seq_t result = 0;
    for (int j = (int)len - 1; j >= 0; --j) {
        uint32_t d = digits[j];
        if (d > 9) {                  /* digits larger than 9 are not allowed */
            result = 0;
            break;
        }
        result = result * 10 + d;
    }

    free(digits);
    free(ndigits);
    return result;
}
