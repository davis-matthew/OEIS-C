/*
 * A395057  a(n) = A241917(A276085(n)).
 * https://oeis.org/A395057
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A241917(A276085(n)), where A276085(n) = Sum_{p^e || n} e
 *            * (p#/p) and A241917(m) = PrimePi(gpf(m)) if m is prime, 0 if m
 *            = 1 or the two largest prime factors of m (with multiplicity)
 *            coincide, else PrimePi(P1) - PrimePi(P2). The intermediate m is
 *            kept in unsigned 128-bit, factored by trial...
 * Range:     u8 n <= 25, u16 n <= 73, u32 n <= 73, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Validated: partial, pass, 88 terms through n = 89 (2026-11-10)
 * Helpers:   factor_u64, is_prime, is_prime_u128, mulmod_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"
#include "../utils/is_prime_u128.h"
#include "../utils/mulmod_u128.h"

typedef unsigned __int128 u128_t;

static u128_t u128_gcd(u128_t x, u128_t y)
{
    while (y != 0) {
        u128_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

static u128_t rho_find_u128(u128_t m)
{
    if (m % 2 == 0)
        return 2;
    for (unsigned c = 1; c < 200; ++c) {
        u128_t x = 2, y = 2, d = 1;
        unsigned long long steps = 0;
        while (d == 1 && steps < 50000000ULL) {
            x = (mulmod_u128(x, x, m) + c) % m;
            y = (mulmod_u128(y, y, m) + c) % m;
            y = (mulmod_u128(y, y, m) + c) % m;
            u128_t diff = (x > y) ? (x - y) : (y - x);
            d = u128_gcd(diff, m);
            ++steps;
        }
        if (d != 1 && d != m)
            return d;
    }
    return m;
}

static void fac_rec_u128(u128_t m, u128_t *out, int *cnt)
{
    if (m <= 1 || *cnt >= 250)
        return;
    if (is_prime_u128(m)) {
        out[(*cnt)++] = m;
        return;
    }
    u128_t d = rho_find_u128(m);
    if (d == m) {
        out[(*cnt)++] = m;
        return;
    }
    fac_rec_u128(d, out, cnt);
    fac_rec_u128(m / d, out, cnt);
}

/* SHARED_HELPER: prime counting function pi(x) via Lucy_Hedgehog method */
static uint64_t lucy_prime_pi(uint64_t x)
{
    if (x < 2)
        return 0;
    uint64_t r = 1;
    while ((r + 1) * (r + 1) <= x)
        ++r;
    while (r * r > x)
        --r;
    int64_t *small = (int64_t *)malloc((size_t)(r + 1) * sizeof(int64_t));
    int64_t *large = (int64_t *)malloc((size_t)(r + 1) * sizeof(int64_t));
    small[0] = 0;
    for (uint64_t i = 1; i <= r; ++i) {
        small[i] = (int64_t)i - 1;
        large[i] = (int64_t)(x / i) - 1;
    }
    large[0] = 0;
    for (uint64_t p = 2; p <= r; ++p) {
        if (small[p] == small[p - 1])
            continue;
        int64_t sp = small[p - 1];
        uint64_t p2 = p * p;
        uint64_t lim = x / p2;
        if (lim > r)
            lim = r;
        for (uint64_t i = 1; i <= lim; ++i) {
            uint64_t d = i * p;
            if (d <= r)
                large[i] -= large[d] - sp;
            else
                large[i] -= small[x / d] - sp;
        }
        for (uint64_t v = r; v >= p2; --v)
            small[v] -= small[v / p] - sp;
    }
    uint64_t res = (uint64_t)large[1];
    free(small);
    free(large);
    return res;
}

/* number of primes q with lo < q <= hi */
static u128_t count_primes_between(u128_t lo, u128_t hi)
{
    if (hi <= lo)
        return 0;
    if (hi - lo <= 2000000) {
        u128_t c = 0;
        for (u128_t v = lo + 1; v <= hi; ++v)
            if (is_prime_u128(v))
                ++c;
        return c;
    }
    return (u128_t)lucy_prime_pi((uint64_t)hi) - (u128_t)lucy_prime_pi((uint64_t)lo);
}

static u128_t A241917_u128(u128_t m)
{
    if (m <= 1)
        return 0;
    u128_t pr[256];
    int cnt = 0;
    for (uint64_t d = 2; d < 20000; d += (d == 2 ? 1 : 2)) {
        if ((u128_t)d * (u128_t)d > m)
            break;
        while (m % d == 0) {
            pr[cnt++] = d;
            m /= d;
        }
    }
    if (m > 1)
        fac_rec_u128(m, pr, &cnt);
    for (int i = 1; i < cnt; ++i) {
        u128_t v = pr[i];
        int j = i - 1;
        while (j >= 0 && pr[j] > v) {
            pr[j + 1] = pr[j];
            --j;
        }
        pr[j + 1] = v;
    }
    if (cnt == 1)
        return count_primes_between(0, pr[0]);
    if (pr[cnt - 1] == pr[cnt - 2])
        return 0;
    return count_primes_between(pr[cnt - 2], pr[cnt - 1]);
}

seq_t a(idx_t n)
{
    const u128_t U128_MAX = ~(u128_t)0;
    u128_t m = 0;
    if (n > 1) {
        uint64_t fp[64], fe[64];
        size_t fc = 0;
        factor_u64((uint64_t)n, fp, fe, &fc);
        for (size_t i = 0; i < fc; ++i) {
            u128_t prod = 1;
            for (uint64_t q = 2; q < fp[i]; ++q) {
                if (is_prime(q)) {
                    if (prod > U128_MAX / q)
                        return 0; /* beyond supported range */
                    prod *= q;
                }
            }
            if (fe[i] != 0 && prod > U128_MAX / fe[i])
                return 0;
            u128_t term = prod * fe[i];
            if (m > U128_MAX - term)
                return 0;
            m += term;
        }
    }
    u128_t r = A241917_u128(m);
    seq_t res = 0;
    /* convert u128 result to seq_t exactly */
    for (int b = 127; b >= 0; --b) {
        res = res * 2;
        if ((r >> b) & 1)
            res += 1;
    }
    return res;
}
