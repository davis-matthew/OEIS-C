/*
 * A319667  Palindromes a(n) = (10^n + 1)*(10^(n+1) + 1).
 * https://oeis.org/A319667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (10^n + 1)*(10^(n+1) + 1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 401 (2026-11-01)
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
    /* Compute 10^n and 10^(2n+1) */
    seq_t pow_n     = pow10(n);
    seq_t pow_2n1   = pow10(2*n + 1);

    /* a(n) = 10^(2n+1) + 11*10^n + 1 */
    return pow_2n1 + pow_n * 11 + 1;
}
