/*
 * A267815  Binary representation of the n-th iteration of the "Rule 221"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^(2n+1) - 1)/9 - 10^(n+1) (n >= 1; a(0)=1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 976 (2026-11-01)
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
        return (seq_t)1;

    idx_t expLarge = 2 * n + 1;
    seq_t largePow = pow10(expLarge);           /* 10^(2n+1) */
    seq_t repunit  = (largePow - 1) / 9;        /* (10^(2n+1)-1)/9  = 111...1 (2n+1 ones) */
    seq_t midPow   = pow10(n + 1);              /* 10^(n+1)         */

    return repunit - midPow;                    /* zero the middle digit */
}
