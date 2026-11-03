/*
 * A261618  Concatenation of n, n+1 and n.
 * https://oeis.org/A261618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(n, n+1, n)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 998, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 52 terms through n = 9864 (2026-11-01)
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
    /* Count decimal digits of n and n+1 */
    idx_t d2 = 0;
    idx_t tmp = n;
    while (tmp) {
        ++d2;
        tmp /= 10;
    }

    idx_t d1 = 0;
    tmp = n + 1;
    while (tmp) {
        ++d1;
        tmp /= 10;
    }

    /* Compute the concatenated value */
    seq_t p12 = pow10(d1 + d2);   /* 10^(d1 + d2) */
    seq_t p2  = pow10(d2);        /* 10^d2 */

    seq_t part1 = ((seq_t)n) * p12;
    seq_t part2 = ((seq_t)(n + 1)) * p2;
    seq_t part3 = (seq_t)n;

    return part1 + part2 + part3;
}
