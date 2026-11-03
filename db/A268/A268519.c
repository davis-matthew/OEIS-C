/*
 * A268519  Odd powers of 2 written between a pair of 1's.
 * https://oeis.org/A268519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(1, 2^(2n-1), 1) where a(n) = 10^(d+1) +
 *            2^(2n-1)*10 + 1 and d = number of decimal digits of 2^(2n-1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 24 (2026-11-01)
 * Helpers:   num_digits, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include utilities used */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_digits.h"
#include "../utils/pow2.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Compute the n-th term of OEIS sequence A268519 */
seq_t a(idx_t n)
{
    /* exponent of 2: 2n - 1 */
    idx_t exp = 2 * n - 1;
    /* compute 2^(2n-1) */
    seq_t x = pow2(exp);

    /* number of decimal digits of x */
    int d = num_digits(x);

    /* 10^(d+1) */
    seq_t pow10part = pow10((idx_t)(d + 1));

    /* a(n) = pow10part + x*10 + 1 */
    return pow10part + x * 10 + 1;
}
