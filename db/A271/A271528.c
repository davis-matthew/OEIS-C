/*
 * A271528  a(n) = 2*(10^n - 1)^2/27.
 * https://oeis.org/A271528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(10^n - 1)^2/27
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 15,
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
    if (n == 0)
        return 0;

    seq_t t = pow10(n);   /* 10^n */
    t -= 1;               /* 10^n - 1, divisible by 9 */
    seq_t r = t / 9;      /* (10^n - 1) / 9 */
    seq_t sq = r * r;     /* r^2 */
    return sq * 6;        /* 6 * r^2 == 2*(10^n - 1)^2/27 */
}
