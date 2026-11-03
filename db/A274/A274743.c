/*
 * A274743  Repunits with odd indices multiplied by 99, i.e., 99*(1, 111,
 *          11111, 1111111, ...).
 * https://oeis.org/A274743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11 * (10^(2*n-1) - 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Index n starts at 1 – a(1) = 99, a(2) = 10989, …) */

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
    if (n == 0)          /* offset-1 guard; not part of the sequence */
        return 0;

    seq_t p = pow10(2 * n - 1); /* 10^(2*n-1) */
    p -= 1;                     /* minus 1 */
    p *= 11;                    /* times 11 */
    return p;
}
