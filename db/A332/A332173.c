/*
 * A332173  a(n) = 7*(10^(2n+1)-1)/9 - 4*10^n.
 * https://oeis.org/A332173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 7*(10^(2n+1)-1)/9 - 4*10^n
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
    /* Compute the repunit R(2n+1) = (10^(2n+1)-1)/9
       by iteratively appending a '1' digit. */
    seq_t rep = 1;                  /* R(1) = 1 */
    for (idx_t i = 0; i < 2 * n; ++i)
        rep = rep * 10 + 1;         /* make next digit 1 */

    /* 7 * R(2n+1)  */
    seq_t term = rep * 7;

    /* 4 * 10^n  */
    seq_t sub = pow10(n) * 4;

    return term - sub;
}
