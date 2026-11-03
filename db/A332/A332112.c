/*
 * A332112  a(n) = (10^(2n+1)-1)/9 + 10^n.
 * https://oeis.org/A332112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (10^(2n+1)-1)/9 + 10^n
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
    /* Compute 10^(2n+1) */
    idx_t exp1 = (n * 2) + 1;
    seq_t pow2n1 = pow10(exp1);
    /* (10^(2n+1)-1)/9 */
    seq_t rep = (pow2n1 - 1) / (seq_t)9;
    /* Compute 10^n */
    seq_t pown = pow10(n);
    /* Sum */
    return rep + pown;
}
