/*
 * A292552  Nontotients of the form 10^k - 2.
 * https://oeis.org/A292552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10^(n+1) - 2 (a(1) = 98, a(2) = 998, …)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-01)
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
    /* The OEIS offset is 1; values for n < 1 are undefined. */
    if (n == 0) return 0;
    seq_t v = pow10(n + 1);
    return v - (seq_t)2;
}
