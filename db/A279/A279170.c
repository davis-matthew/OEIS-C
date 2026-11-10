/*
 * A279170  a(n) is the smallest among the natural numbers m with the property
 *          that there exists a non-constant quadratic map S^n -> S^m from the
 *          n-dimensional sphere to the m-dimensional sphere.
 * https://oeis.org/A279170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2^t + m) = 2^t if 0 <= m < A003484(2^t); a(2^t + m) = 2^t +
 *            a(m) if A003484(2^t) <= m < 2^t
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A279170_rho_pow2(idx_t v2)
{
    idx_t b = v2 / 4;
    idx_t c = v2 % 4;
    return 8 * b + ((idx_t)1 << c);
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    while (n > 0) {
        idx_t t = 1;
        idx_t v2 = 0;
        while (t <= (n >> 1)) {
            t <<= 1;
            v2++;
        }
        idx_t m = n - t;
        result += (seq_t)t;
        if (m < A279170_rho_pow2(v2))
            break;
        n = m;
    }
    return result;
}
