/*
 * A332196  a(n) = 10^(2n+1) - 1 - 3*10^n.
 * https://oeis.org/A332196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10^(2n+1) - 1 - 3*10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-01)
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
    /* compute 10^(2n+1) and 10^n using pow10 */
    seq_t pow1 = pow10((idx_t)(2*n + 1));
    seq_t pow2 = pow10((idx_t)n);
    /* a(n) = pow1 - 1 - 3*pow2 */
    return pow1 - 1 - 3 * pow2;
}
