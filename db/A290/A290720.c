/*
 * A290720  a(n) = 2*3^n + 4^n + 3*n.
 * https://oeis.org/A290720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*3^n + 4^n + 3*n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 1626 (2026-11-01)
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow3.h"

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Compute 2*3^n + 4^n + 3*n, using the provided power helpers. */
    seq_t three_pow   = pow3(n);          /* 3^n   */
    seq_t four_pow    = pow4(n);          /* 4^n   */
    seq_t two_three   = three_pow * 2;    /* 2*3^n */
    seq_t three_n     = 3 * (seq_t)n;     /* 3*n   */
    return two_three + four_pow + three_n;
}
