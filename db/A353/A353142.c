/*
 * A353142  Decimal repunits written in base 3.
 * https://oeis.org/A353142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = decimal interpretation of the base‑3 expansion of the
 *            decimal repunit of length n: A007089(A002275(n))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) is defined as 0 */
    if (n == 0) return 0;

    /* Build the decimal repunit R_n = (10^n - 1)/9 by repeated
       multiplication and addition.  This works for any seq_t
       that overflows modulo 2^k as the result is only needed
       when it fits in seq_t. */
    seq_t rep = 0;
    for (idx_t i = 0; i < n; ++i)
        rep = rep * 10 + 1;

    /* Convert rep to base‑3 digits; least significant digit first.
       The number of base‑3 digits is at most 2*n for all seq_t
       that can hold rep.  Use a small dynamic array. */
    size_t cap = 2 * n + 1;
    uint8_t *digits = (uint8_t *)malloc(cap);
    if (!digits) return 0;   /* allocation failure – unlikely */

    size_t len = 0;
    seq_t tmp = rep;
    while (tmp > 0) {
        seq_t rem = tmp % (seq_t)3;
        tmp   = tmp / (seq_t)3;
        digits[len++] = (uint8_t)rem;
    }

    /* Build the final decimal integer from the base‑3 digits
       (now interpreted as decimal digits). */
    seq_t result = 0;
    for (size_t i = 0; i < len; ++i)
        result = result * 10 + digits[len - 1 - i];

    free(digits);
    return result;
}
