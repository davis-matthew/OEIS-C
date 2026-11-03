/*
 * A295430  a(n) is the least nontrivial multiple of n that begins with 3.
 * https://oeis.org/A295430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least nontrivial multiple of n that begins with digit 3
 * Range:     u8 n <= 13, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9439 (2026-11-01)
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
    /* n >= 1 */
    seq_t k = 2;                /* first nontrivial multiple factor */
    while (1) {
        seq_t m = (seq_t)n * k;   /* current multiple of n */

        /* find leading digit of m */
        int d = num_digits(m);      /* number of decimal digits */
        seq_t div = pow10(d - 1);  /* 10^(d-1) */
        seq_t first = m / div;

        if (first == 3)            /* first digit is 3 */
            return m;

        ++k;                       /* try next multiple */
    }
}
