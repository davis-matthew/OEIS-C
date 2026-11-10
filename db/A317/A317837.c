/*
 * A317837  a(n) = Sum_{d|n, d<n} A002487(d).
 * https://oeis.org/A317837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d<n} A002487(d), with Stern's diatomic
 *            A002487(0)=0, A002487(1)=1, A002487(2m)=A002487(m),
 *            A002487(2m+1)=A002487(m)+A002487(m+1)
 * Range:     u8 n <= 1259, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 111 terms through n = 8998 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A002487_stern(idx_t m) {
    seq_t x = 1, y = 0;
    while (m > 0) {
        if (m & 1) y += x;
        else x += y;
        m >>= 1;
    }
    return y;
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    if (n <= 1) return 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;
            if (d < n) sum += A002487_stern(d);
            if (e != d && e < n) sum += A002487_stern(e);
        }
    }
    return sum;
}
