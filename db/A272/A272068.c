/*
 * A272068  a(n) = (10^n-1)^5.
 * https://oeis.org/A272068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^n - 1)^5
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 1, u64 n <= 3, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 10 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Computes the nth term of OEIS A272068: a(n) = (10^n-1)^5.
 * This works for any seq_t type (fixed-width or arbitrary-precision)
 * as long as standard arithmetic operators are defined. */
seq_t a(idx_t n)
{
    /* 10^n */
    seq_t ten_pow = pow10(n);

    /* 10^n - 1 */
    seq_t sub = ten_pow - (seq_t)1;

    /* (10^n - 1)^5 = ((10^n-1) * ... * (10^n-1)) 5 times */
    seq_t res = sub;
    res = res * sub;  /* ^2 */
    res = res * sub;  /* ^3 */
    res = res * sub;  /* ^4 */
    res = res * sub;  /* ^5 */

    return res;
}
