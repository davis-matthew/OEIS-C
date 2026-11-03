/*
 * A274766  Multiplication of pair of contiguous repunits, i.e., (0*1, 1*11,
 *          11*111, 111*1111, 1111*11111, ...).
 * https://oeis.org/A274766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^n - 1)*(10^(n+1) - 1)/81
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 496 (2026-11-01)
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
    seq_t ten_pow_n   = pow10(n);
    seq_t ten_pow_n1  = pow10(n + 1);
    seq_t rep_n   = (ten_pow_n - 1) / 9;
    seq_t rep_n1  = (ten_pow_n1 - 1) / 9;
    return rep_n * rep_n1;
}
