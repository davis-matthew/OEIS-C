/*
 * A332192  a(n) = 10^(2*n+1) - 1 - 7*10^n.
 * https://oeis.org/A332192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10^(2*n+1) - 1 - 7*10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-01)
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
    /* Compute powers of ten.  pow10 is assumed to work for any idx_t exponent
       and return the result as seq_t.  For << values the intermediate values
       stay within the capabilities of seq_t. */
    idx_t e1 = 2*n + 1;     /* exponent for 10^(2*n+1) */
    idx_t e2 = n;           /* exponent for 10^n */

    seq_t p1 = pow10(e1);   /* 10^(2*n+1) */
    seq_t p2 = pow10(e2);   /* 10^n */

    /* Return 10^(2*n+1) - 1 - 7*10^n */
    return p1 - 1 - 7*p2;
}
