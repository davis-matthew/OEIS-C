/*
 * A284377  Concatenation of the first n positive 4th powers.
 * https://oeis.org/A284377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = concatenation of 1^4, 2^4, …, n^4
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/num_digits.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t power4 = (seq_t)k * (seq_t)k * (seq_t)k * (seq_t)k;
        idx_t digits = (idx_t)num_digits(power4);
        result = result * pow10(digits) + power4;
    }
    return result;
}
