/*
 * A248555  a(n) is the concatenation of 3n-2, 3n and 3n-1.
 * https://oeis.org/A248555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = concat(3n-2, 3n, 3n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 333, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 47 terms through n = 993 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Concatenates the decimal representations of 3n-2, 3n, and 3n-1. */
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
    seq_t tn = (seq_t)n;          /* convert n to seq_t to avoid overflow */
    seq_t a1 = tn * (seq_t)3 - (seq_t)2; /* 3n-2 */
    seq_t b1 = tn * (seq_t)3;           /* 3n   */
    seq_t c1 = tn * (seq_t)3 - (seq_t)1;/* 3n-1 */

    int len_b = num_digits(b1);         /* number of decimal digits of 3n */
    int len_c = num_digits(c1);         /* number of decimal digits of 3n-1 */

    seq_t res = a1 * pow10((idx_t)(len_b + len_c))
              + b1 * pow10((idx_t)(len_c))
              + c1;
    return res;
}
