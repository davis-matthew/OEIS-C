/*
 * A272896  Difference between the number of odd and even digits in the
 *          decimal expansion of 2^n.
 * https://oeis.org/A272896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = (# odd digits in 2^n) – (# even digits in 2^n)
 * Range:     i8 n <= 5505, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Upper bound on number of decimal digits of 2^n:
       floor(n*log10(2)) + 1  < 0.402*n + 2   */
    size_t max_digits = (size_t)(0.402 * (double)n) + 5;
    uint8_t *digits = (uint8_t *)malloc(max_digits);
    if (!digits) return 0;                 /* fallback on allocation failure */

    size_t len = 1;          /* current number of digits */
    digits[0] = 1;           /* 2^0 = 1 */

    /* Build 2^n by repeated multiplication by 2 in base 10 */
    for (idx_t i = 0; i < n; ++i) {
        uint8_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            uint8_t tmp = digits[j] * 2 + carry;
            digits[j] = tmp % 10;
            carry = tmp / 10;
        }
        if (carry) {
            digits[len++] = carry;
        }
    }

    /* Count odd and even digits */
    seq_t odd = 0, even = 0;
    for (size_t j = 0; j < len; ++j) {
        if (digits[j] & 1)
            ++odd;
        else
            ++even;
    }

    free(digits);
    return odd - even;
}
