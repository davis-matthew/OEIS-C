/*
 * A279085  Number of distinct residues of triangular numbers mod 10^n.
 * https://oeis.org/A279085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = (5*10^n + (9 - 2*(-1)^n)*2^n)/12
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 8 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow2.h"

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
    seq_t p10   = pow10(n);            /* 10^n */
    seq_t p2    = pow2(n);             /* 2^n */

    /* 9 - 2*(-1)^n equals 7 when n is even, 11 when n is odd */
    seq_t coeff = ((n & (idx_t)1) == 0) ? (seq_t)7 : (seq_t)11;

    seq_t numerator = (seq_t)5 * p10 + coeff * p2;
    return numerator / (seq_t)12;
}
