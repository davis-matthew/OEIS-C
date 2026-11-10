/*
 * A294038  a(n) = Product_{k=0..n-1} n/gcd(n, A056040(k)).
 * https://oeis.org/A294038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n-1} n/gcd(n, A056040(k)), computed via
 *            p-adic valuations of A056040(k) = k!/(floor(k/2)!)^2 (Legendre)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A294038_leg(uint64_t k, uint64_t p)
{
    uint64_t s = 0;
    while (k) {
        k /= p;
        s += k;
    }
    return s;
}

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;
    if (n == 0)
        return res;

    uint64_t pr[20];
    uint64_t ex[20];
    int cnt = 0;
    uint64_t m = (uint64_t)n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            pr[cnt] = p;
            ex[cnt] = e;
            ++cnt;
        }
    }
    if (m > 1) {
        pr[cnt] = m;
        ex[cnt] = 1;
        ++cnt;
    }

    for (uint64_t k = 0; k < (uint64_t)n; ++k) {
        seq_t g = (seq_t)1;
        for (int i = 0; i < cnt; ++i) {
            uint64_t v = A294038_leg(k, pr[i]) - 2 * A294038_leg(k / 2, pr[i]);
            uint64_t e = v < ex[i] ? v : ex[i];
            for (uint64_t j = 0; j < e; ++j)
                g *= (seq_t)pr[i];
        }
        res *= (seq_t)n / g;
    }
    return res;
}
