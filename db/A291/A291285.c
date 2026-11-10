/*
 * A291285  Expansion of G(x)^4 where G(x) = g.f. for A291096.
 * https://oeis.org/A291285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^n * binomial(4n+4, n) / (n+1) (Lagrange inversion of G
 *            = 1 + 3x*G^4, [x^n] G^4)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 646 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t top = (seq_t)4 * nn + (seq_t)4;
    seq_t res = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        res = res * (top - (seq_t)k + (seq_t)1) / (seq_t)k;
    }
    res = res / (nn + (seq_t)1);
    for (idx_t k = 1; k <= n; ++k) {
        res *= (seq_t)3;
    }
    return res;
}
