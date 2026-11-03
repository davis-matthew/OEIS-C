/*
 * A248422  Even integers concatenated with themselves.
 * https://oeis.org/A248422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2*n*10^ceil(log10(2*n)) + 2*n
 * Range:     u8 n <= 4, u16 n <= 49, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 66 terms through n = 949 (2026-11-01)
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
    seq_t v = (seq_t)n * 2;          /* even integer 2*n */
    int d = num_digits(v);           /* number of decimal digits of 2*n */
    seq_t p = pow10((idx_t)d);      /* 10^d */
    return v * p + v;               /* concatenate v with itself */
}
