/*
 * A346629  Number of n-digit positive integers that are the product of two
 *          integers ending with 2.
 * https://oeis.org/A346629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   a(1) = 1, a(2) = 4, a(n) = 45*10^(n-3) for n > 2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided utility function pow10(idx_t). */

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
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)4;
    /* n > 2 */
    seq_t p = pow10(n - 3);          /* 10^(n-3) */
    return p * (seq_t)45;            /* 45 * 10^(n-3) */
}
