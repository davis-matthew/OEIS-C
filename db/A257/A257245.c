/*
 * A257245  Odd bisection of A257244: a(n) = A257244(2n - 1).
 * https://oeis.org/A257245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A006530(A256393(2n-1)); A256393: b(1)=2,
 *            b(k)=b(k-1)+gpf(b(k-1)) for even k, b(k)=b(k-1)+lpf(b(k-1)) for
 *            odd k
 * Range:     u8 n <= 23, u16 n <= 370, u32 n <= 1992, u64 n <= 2048,
 *            u128 n <= 2048, big any n
 * Validated: standard, pass, 97 terms through n = 2038 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static void A257245_pf(seq_t m, seq_t *lo, seq_t *hi)
{
    seq_t l = 0, g = 1;
    while (m % 2 == 0) {
        if (l == 0) l = 2;
        g = 2;
        m = m / 2;
    }
    for (seq_t i = 3; i * i <= m; i += 2) {
        while (m % i == 0) {
            if (l == 0) l = i;
            g = i;
            m = m / i;
        }
    }
    if (m > 1) {
        if (l == 0) l = m;
        g = m;
    }
    *lo = l;
    *hi = g;
}

seq_t a(idx_t n)
{
    seq_t t = 2;
    seq_t lo, hi;
    idx_t last = 2 * n - 1;
    for (idx_t k = 2; k <= last; k++) {
        A257245_pf(t, &lo, &hi);
        if ((k & 1) == 0) t = t + hi;
        else t = t + lo;
    }
    A257245_pf(t, &lo, &hi);
    return hi;
}
