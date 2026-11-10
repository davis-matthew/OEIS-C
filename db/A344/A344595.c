/*
 * A344595  Numbers k such that A011772(k) > A344878(k) and A011772(k) is a
 *          divisor of A344875(k).
 * https://oeis.org/A344595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A011772(k) > A344878(k) and A011772(k) |
 *            A344875(k); found by direct search over k
 * Range:     u8 none, u16 n <= 28, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A344595_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static uint64_t A344595_inv(uint64_t d, uint64_t e)
{
    /* inverse of d modulo e (e > 1, gcd(d,e)=1) */
    int64_t r0 = (int64_t)e, r1 = (int64_t)(d % e);
    int64_t t0 = 0, t1 = 1;
    while (r1) {
        int64_t q = r0 / r1;
        int64_t r2 = r0 - q * r1; r0 = r1; r1 = r2;
        int64_t t2 = t0 - q * t1; t0 = t1; t1 = t2;
    }
    if (t0 < 0) t0 += (int64_t)e;
    return (uint64_t)t0;
}

static bool A344595_test(uint64_t k)
{
    uint64_t pw[24];     /* prime powers of 2k */
    int np = 0;
    uint64_t prod = 1, L = 1;
    uint64_t t = k;
    uint64_t e2 = 0;
    while ((t & 1) == 0) { t >>= 1; ++e2; }
    {
        uint64_t q = 2;
        for (uint64_t i = 0; i < e2 + 1; ++i) q *= 2;
        /* q = 2^(e2+2); 2k has 2^(e2+1) */
        q >>= 1;
        pw[np++] = q;
        uint64_t f = (q * 2) - 1;       /* 2^(e2+1)... fix below */
        (void)f;
        f = q - 1;                      /* 2^(e2+1) - 1 */
        prod *= f;
        L = (L / A344595_gcd(L, f)) * f;
    }
    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            uint64_t q = 1;
            while (t % p == 0) { t /= p; q *= p; }
            pw[np++] = q;
            uint64_t f = q - 1;
            prod *= f;
            L = (L / A344595_gcd(L, f)) * f;
        }
    }
    if (t > 1) {
        pw[np++] = t;
        uint64_t f = t - 1;
        prod *= f;
        L = (L / A344595_gcd(L, f)) * f;
    }
    /* A011772(k): smallest m>=1 with m(m+1) = 0 mod 2k */
    uint64_t N = 2 * k;
    uint64_t best = N - 1;
    for (uint32_t mask = 0; mask < (1u << np); ++mask) {
        uint64_t d = 1;
        for (int i = 0; i < np; ++i) if (mask >> i & 1) d *= pw[i];
        uint64_t e = N / d;
        uint64_t m;
        if (e == 1) m = d;
        else {
            uint64_t tt = e - A344595_inv(d, e);
            tt %= e;
            m = d * tt;
            if (m == 0) continue;
        }
        if (m < best) best = m;
    }
    if (best <= L) return false;
    return prod % best == 0;
}

seq_t a(idx_t n)
{
    uint64_t cnt = 0;
    for (uint64_t k = 1;; ++k) {
        if (A344595_test(k)) {
            if (++cnt == (uint64_t)n) return (seq_t)k;
        }
    }
}
