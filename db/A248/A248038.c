/*
 * A248038  3n concatenated with itself.
 * https://oeis.org/A248038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenate(3*n, 3*n) in base‑10, i.e. a(n) = 3*n *
 *            10^digits(3*n) + 3*n
 * Range:     u8 n <= 3, u16 n <= 33, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-01)
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
    /* compute 3*n in the target integer type */
    seq_t three_n = (seq_t)n * (seq_t)3;

    /* number of decimal digits of 3*n */
    int d = num_digits(three_n);

    /* 10^d as a seq_t */
    seq_t pow10d = pow10((idx_t)d);

    /* concatenate by multiplication and addition */
    return three_n * pow10d + three_n;
}
