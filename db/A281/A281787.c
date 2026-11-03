/*
 * A281787  a(n) = sum of all numbers between 1 and 10^n that are divisible by
 *          3 or 5.
 * https://oeis.org/A281787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*100^n - 5*10^n + 40)/30
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 489 (2026-11-01)
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
    /* Compute 100^n as 10^(2n) */
    seq_t term1 = (seq_t)7 * pow10(2*n);      /* 7 * 100^n */
    seq_t term2 = (seq_t)5 * pow10(n);        /* 5 * 10^n  */
    seq_t num   = term1 - term2 + (seq_t)40;  /* numerator: 7*100^n - 5*10^n + 40 */
    return num / (seq_t)30;                   /* divide by 30 to get a(n) */
}
