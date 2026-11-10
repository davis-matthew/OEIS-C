/*
 * A272533  Numbers n such that A072499(n)!=A072504(n).
 * https://oeis.org/A272533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m such that A072499(m) != A072504(m); search:
 *            product and lcm of the divisors d <= sqrt(m) differ iff two of
 *            them share a factor > 1
 * Range:     u8 n <= 96, u16 n <= 5600, u32 n <= 5600, u64 n <= 5600,
 *            u128 n <= 5600, big any n
 * Validated: standard, pass, 75 terms through n = 5326 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static idx_t A272533_gcd(idx_t x, idx_t y) {
    while (y) { idx_t t = x % y; x = y; y = t; }
    return x;
}

static int A272533_differs(idx_t m) {
    idx_t divs[512];
    int cnt = 0;
    for (idx_t d = 2; d * d <= m; ++d) {
        if (m % d == 0) {
            for (int i = 0; i < cnt; ++i)
                if (A272533_gcd(divs[i], d) > 1) return 1;
            if (cnt < 512) divs[cnt++] = d;
        }
    }
    return 0;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t m = 0;
    while (count < n) {
        ++m;
        if (A272533_differs(m)) ++count;
    }
    return (seq_t)m;
}
