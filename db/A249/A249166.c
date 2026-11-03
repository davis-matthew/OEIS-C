/*
 * A249166  Odd integers concatenated with themselves.
 * https://oeis.org/A249166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(2n-1, 2n-1) (concatenation in base 10)
 * Range:     u8 n <= 5, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
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
    /* odd integer 2n-1 */
    seq_t odd = (seq_t)n * (seq_t)2 - (seq_t)1;

    /* number of decimal digits of odd */
    int d = num_digits(odd);

    /* 10^d as seq_t */
    seq_t pow10d = pow10((idx_t)d);

    /* concatenation: odd * 10^d + odd */
    return odd * pow10d + odd;
}
