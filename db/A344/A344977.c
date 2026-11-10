/*
 * A344977  Numbers k for which A011772(k) exceeds A344878(k).
 * https://oeis.org/A344977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A011772(k) > A344878(k), found by direct
 *            search over k
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A344977_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static int A344977_test(uint64_t k)
{
    /* A011772(k) */
    uint64_t m1 = 0;
    for (uint64_t m = 1; m < 2 * k; ++m) {
        unsigned __int128 t = (unsigned __int128)m * (m + 1) / 2;
        if (t % k == 0) { m1 = m; break; }
    }
    if (m1 == 0) m1 = 2 * k - 1;

    /* A344878(k): lcm only grows, so once it reaches m1 the test fails */
    uint64_t l = 1, r = k;
    for (uint64_t p = 2; p * p <= r || r > 1; ++p) {
        if (p * p > r) p = r;
        if (r % p == 0) {
            uint64_t pe = 1;
            while (r % p == 0) { r /= p; pe *= p; }
            if (p == 2) pe *= 2;
            uint64_t v = pe - 1;
            uint64_t g = A344977_gcd(l, v);
            l = (l / g) * v;
            if (l >= m1) return 0;
        }
    }
    return m1 > l;
}

seq_t a(idx_t n)
{
    uint64_t cnt = 0, k = 0;
    while (cnt < (uint64_t)n) {
        ++k;
        if (A344977_test(k)) ++cnt;
    }
    return (seq_t)k;
}
