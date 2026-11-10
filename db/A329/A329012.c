/*
 * A329012  a(n) = p(1,n), where p(x,n) is the strong divisibility sequence of
 *          polynomials based on sqrt(5) as in A327322.
 * https://oeis.org/A329012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^n - 4^n) / (6*g(n)), where g(n) is the 3-smooth part
 *            of n. This is p(1,n) for p(x,n) = ((5x+5)^n -
 *            (5x-1)^n)/(6*content), the strong divisibility sequence of
 *            A327322. g(n) = 3-smooth part of n was inferred from the 21
 *            listed terms, not proved. Computed as (2^(n-1)/g2) * ((5^n -
 *            2^n)/(3...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    idx_t g2 = 1, g3 = 1, m = n;
    while ((m & 1) == 0) { m >>= 1; g2 <<= 1; }
    while (m % 3 == 0)   { m /= 3; g3 *= 3; }

    seq_t p2 = (seq_t)1;
    for (idx_t i = 1; i < n; ++i) p2 = p2 * (seq_t)2;   /* 2^(n-1) */
    p2 = p2 / (seq_t)g2;

    seq_t p5 = (seq_t)1, q2 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        p5 = p5 * (seq_t)5;
        q2 = q2 * (seq_t)2;
    }
    seq_t d = (p5 - q2) / (seq_t)3;
    d = d / (seq_t)g3;

    return p2 * d;
}
