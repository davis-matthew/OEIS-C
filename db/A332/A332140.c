/*
 * A332140  a(n) = 4*(10^(2*n+1) - 1)/9 - 4*10^n.
 * https://oeis.org/A332140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 4*(10^(2*n+1) - 1)/9 - 4*10^n
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 15,
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
    /* Compute 10^n and 10^(n+1) */
    seq_t pow_n  = pow10(n);
    seq_t pow_n1 = pow10(n + 1);

    /* left = 4*(10^n - 1)/9  -> number consisting of n fours */
    seq_t left = (pow_n - (seq_t)1) / (seq_t)9;
    left = left * (seq_t)4;

    /* a(n) = left * 10^(n+1) + left = left*(10^(n+1)+1) */
    return left * pow_n1 + left;
}
