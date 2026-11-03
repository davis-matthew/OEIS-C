/*
 * A328683  Positive integers that are equal to 99...99 (repdigit with n
 *          digits 9) times the sum of their digits.
 * https://oeis.org/A328683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 9 * n * (10^n - 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 982 (2026-11-01)
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
    /* The sequence is defined for n >= 1; return 0 for n == 0 to be safe. */
    if (n == 0) return 0;

    seq_t ten_pow = pow10(n);          /* 10^n */
    seq_t result = (seq_t)9 * (seq_t)n * (ten_pow - 1);
    return result;
}
