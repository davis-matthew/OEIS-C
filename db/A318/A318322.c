/*
 * A318322  Additive with a(p^e) = A007306(e).
 * https://oeis.org/A318322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p^e || n} A007306(e), where A007306(e) =
 *            A002487(2e-1) for e>0 (additive, a(1)=0)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9958 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A007306_stern(idx_t m) {
    seq_t x = 1, y = 0;
    while (m > 0) {
        if (m & 1) y += x;
        else x += y;
        m >>= 1;
    }
    return y;
}

static seq_t A007306_a(idx_t n) {
    if (n == 0) return 1;
    return A007306_stern(2 * n - 1);
}

seq_t a(idx_t n) {
    seq_t total = 0;
    idx_t p;
    for (p = 2; p <= n / p; p++) {
        if (n % p == 0) {
            idx_t e = 0;
            while (n % p == 0) {
                n /= p;
                e++;
            }
            total += A007306_a(e);
        }
    }
    if (n > 1) total += A007306_a(1);
    return total;
}
