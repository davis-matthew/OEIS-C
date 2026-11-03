/*
 * A393794  Fixed points of the 1089-trick map T(n) = |n - reverse(n)| +
 *          reverse(|n - reverse(n)|).
 * https://oeis.org/A393794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 110 * (10^(n+1) - 1)
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-01)
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
    if (n < 1) return 0;
    seq_t r = pow10(n + 1);   /* 10^(n+1) */
    r -= (seq_t)1;            /* 10^(n+1) - 1 */
    return r * (seq_t)110;    /* 110 * (10^(n+1) - 1) */
}
