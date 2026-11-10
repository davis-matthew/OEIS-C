/*
 * A329010  a(n) = p(2,n), where p(x,n) is the strong divisibility sequence of
 *          polynomials based on sqrt(3) as in A327321.
 * https://oeis.org/A329010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (9^n - 5^n) / 2^(2+v2(n)), where v2 is the 2-adic
 *            valuation of n (a(1) = 1, a(2) = 7)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A329010_pw(seq_t b, idx_t e)
{
    seq_t r = 1;
    while (e) {
        if (e & 1) r = r * b;
        e >>= 1;
        if (e) b = b * b;
    }
    return r;
}

seq_t a(idx_t n)
{
    unsigned s = 0;
    idx_t m = n;
    while ((m & 1) == 0) { m >>= 1; s++; }
    seq_t X = A329010_pw((seq_t)9, m);
    seq_t Y = A329010_pw((seq_t)5, m);
    seq_t r = (X - Y) / 4;
    for (unsigned i = 0; i < s; i++) {
        r = r * ((X + Y) / 2);
        X = X * X;
        Y = Y * Y;
    }
    return r;
}
