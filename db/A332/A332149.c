/*
 * A332149  a(n) = 4*(10^(2*n+1)-1)/9 + 5*10^n.
 * https://oeis.org/A332149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 4*(10^(2*n+1)-1)/9 + 5*10^n
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
    /* 10^(2*n+1)  */
    idx_t exp1 = 2*n + 1;
    seq_t pow21 = pow10(exp1);

    /* (10^(2*n+1)-1)/9  * 4  */
    seq_t term1 = (pow21 - 1) / 9;   /* division is exact */
    term1 *= 4;

    /* 5*10^n  */
    seq_t term2 = pow10(n);
    term2 *= 5;

    return term1 + term2;
}
