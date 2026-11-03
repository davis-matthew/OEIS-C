/*
 * A332180  a(n) = 8*(10^(2n+1)-1)/9 - 8*10^n.
 * https://oeis.org/A332180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 8 * (10^(2*n+1) - 1) / 9 - 8 * 10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The computation uses the pow10 helper to obtain 10^k as a seq_t value.
   To avoid overflow on 64‑bit seq_t, (10^(2n+1)-1) is first divided by 9,
   then multiplied by 8.  All intermediate results fit in seq_t for all
   n whose final value fits in seq_t. */

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
    /* 10^n */
    seq_t pow_n = pow10(n);

    /* 10^(2n+1) = 10 * (10^n)^2 */
    seq_t pow_2n1 = pow_n;
    pow_2n1 *= pow_n;       /* (10^n)^2 = 10^(2n)  */
    pow_2n1 *= (seq_t)10;   /* times 10 gives 10^(2n+1) */

    /* term = 8 * (10^(2n+1) - 1) / 9 */
    seq_t term = (pow_2n1 - (seq_t)1) / (seq_t)9; /* 111…111 with 2n+1 ones */
    term *= (seq_t)8;                                 /* 888…888 */

    /* subtract 8 * 10^n */
    seq_t sub = pow_n * (seq_t)8;
    return term - sub;
}
