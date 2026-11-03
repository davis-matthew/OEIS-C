/*
 * A248338  a(n) = 10^n - 4^n.
 * https://oeis.org/A248338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n - 4^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 910 (2026-11-01)
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

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t v10 = pow10(n);
    seq_t v4  = pow4(n);
    return v10 - v4;
}
