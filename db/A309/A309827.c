/*
 * A309827  a(n) is the square of the number consisting of one 1 and n 6's:
 *          (166...6)^2.
 * https://oeis.org/A309827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ((5*10^n - 2)/3)^2
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 466 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the square of the decimal number consisting of one 1 and n 6's. */
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
    seq_t pow10n = pow10(n);               /* 10^n as a seq_t */
    seq_t v = ((seq_t)5 * pow10n - (seq_t)2) / (seq_t)3;
    return v * v;
}
