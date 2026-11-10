/*
 * A323072  a(n) = n/A323071(n) = n/gcd(n, 1+A060681(n)).
 * https://oeis.org/A323072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n/gcd(n, 1+A060681(n)), where A060681(n) = n - n/spf(n),
 *            A060681(1) = 0
 * Range:     u8 n <= 255, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 110 terms through n = 9081 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A060681_spf(idx_t m)
{
    if (m <= 1)
        return 1;
    if ((m & 1) == 0)
        return 2;
    for (idx_t i = 3; (i * i) <= m; i += 2)
        if ((m % i) == 0)
            return i;
    return m;
}

static seq_t A060681_a(idx_t n)
{
    if (n <= 1)
        return 0;
    idx_t p = A060681_spf(n);
    seq_t N = (seq_t)n;
    seq_t P = (seq_t)p;
    seq_t Q = N / P;
    return N - Q;
}

static seq_t A323072_gcd(seq_t x, seq_t y)
{
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t v = A060681_a(n) + 1;
    seq_t g = A323072_gcd(N, v);
    return N / g;
}
