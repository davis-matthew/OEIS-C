/*
 * A377192  Smallest number with the property that you have to change at least
 *          n digits to get a palindrome.
 * https://oeis.org/A377192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10^(2*n-1) + (10^n-1)/9 - 1, a(0) = 0
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 422 (2026-11-01)
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
    if (n == 0)
        return 0;

    /* Compute 10^(2*n-1) */
    idx_t exp = 2 * n - 1;
    seq_t term1 = pow10(exp);

    /* Compute (10^n-1)/9 */
    seq_t term2 = (pow10(n) - 1) / 9;

    return term1 + term2 - 1;
}
