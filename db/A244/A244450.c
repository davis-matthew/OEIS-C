/*
 * A244450  Powers of ten billion.
 * https://oeis.org/A244450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10000000000^n
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 1, u128 n <= 3,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 6 (2026-11-01)
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

seq_t a(idx_t n)
{
    /* 10000000000 == 10^10, so a(n) = (10^10)^n = 10^(10*n) */
    idx_t exp = n * 10;          /* exponent = 10 * n */
    return pow10(exp);           /* pow10(e) returns 10^e */
}
