/*
 * A372010  a(n) is the n-digit number k such that R(k)/k is maximal for any
 *          n-digit number.
 * https://oeis.org/A372010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^(n-1) + 10^(floor(n/2)) - 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-01)
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
    if (n == 0) return (seq_t)0; /* offset is 1; n should never be 0 */
    idx_t e1 = n - 1;          /* n-1 */
    idx_t e2 = n / 2;          /* floor(n/2) */
    seq_t term = pow10(e1) + pow10(e2) - (seq_t)1;
    return term;
}
