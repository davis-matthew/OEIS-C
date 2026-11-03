/*
 * A332181  a(n) = 8*(10^(2n+1)-1)/9 - 7*10^n.
 * https://oeis.org/A332181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 8*(10^(2n+1)-1)/9 - 7*10^n
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
    /* 10^n is needed twice */
    seq_t pow_n = pow10(n);

    /* compute exponent 2*n+1 safely */
    idx_t exp_2np1 = n * (idx_t)2 + (idx_t)1;
    seq_t pow_2np1 = pow10(exp_2np1);

    /* first part: ((10^(2n+1)-1)/9)*8  (division is exact) */
    seq_t term1 = ((pow_2np1 - (seq_t)1) / (seq_t)9) * (seq_t)8;

    /* second part: 7*10^n */
    seq_t term2 = (seq_t)7 * pow_n;

    return term1 - term2;
}
