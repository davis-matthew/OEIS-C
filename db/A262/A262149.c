/*
 * A262149  Rocket sequence 50: a(0)=50, a(n)=A073846(a(n-1)).
 * https://oeis.org/A262149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=50, a(n)=A073846(a(n-1)), where A073846(1)=1,
 *            A073846(2k)=prime(k), A073846(2k+1)=k-th composite; computed via
 *            exact prime counting (Lucy_Hedgehog) plus a segmented sieve
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 90, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Validated: standard, pass, 61 terms through n = 96 (2026-11-07)
 * Helpers:   prime_pi_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include "../utils/prime_pi_u64.h"

static uint32_t *rk_small_pi = NULL;
#define RK_SMALL 1000000u

static uint64_t rk_isqrt(uint64_t x)
{
    uint64_t r = (uint64_t)sqrt((double)x);
    while (r * r > x) --r;
    while ((r + 1) * (r + 1) <= x) ++r;
    return r;
}

static void rk_init_small(void)
{
    if (rk_small_pi) return;
    bool *c = (bool *)calloc(RK_SMALL + 1, sizeof(bool));
    rk_small_pi = (uint32_t *)malloc((RK_SMALL + 1) * sizeof(uint32_t));
    if (!c || !rk_small_pi) abort();
    for (uint64_t i = 2; i * i <= RK_SMALL; ++i)
        if (!c[i]) for (uint64_t j = i * i; j <= RK_SMALL; j += i) c[j] = true;
    uint32_t cnt = 0;
    rk_small_pi[0] = 0; rk_small_pi[1] = 0;
    for (uint64_t i = 2; i <= RK_SMALL; ++i) {
        if (!c[i]) ++cnt;
        rk_small_pi[i] = cnt;
    }
    free(c);
}

static uint64_t rk_pi(uint64_t x)
{
    rk_init_small();
    if (x <= RK_SMALL) return rk_small_pi[x];
    return prime_pi_u64(x);   /* db/utils/prime_pi_u64.h */
}

static uint64_t rk_cnt(uint64_t x, bool want_prime)
{
    if (x < 2) return 0;
    uint64_t p = rk_pi(x);
    return want_prime ? p : (x - 1 - p);
}

#define RK_BL (1u << 20)

/* k-th prime (want_prime) or k-th composite, k >= 1 */
static uint64_t rk_find(uint64_t k, bool want_prime)
{
    double dk = (double)k, x;
    if (want_prime) {
        if (k < 6) x = 15;
        else {
            double l = log(dk), ll = log(l);
            x = dk * (l + ll - 1.0 + (ll - 2.0) / l);
        }
    } else {
        x = (k < 3) ? 10.0 : dk + dk / log(dk) + 10.0;
    }
    if (x < 10) x = 10;
    uint64_t xi = (uint64_t)x, c = 0;
    for (int it = 0; it < 10; ++it) {
        c = rk_cnt(xi, want_prime);
        double lx = log((double)xi);
        double slope = want_prime ? 1.0 / lx : 1.0 - 1.0 / lx;
        double step = ((double)k - (double)c) / slope;
        if (fabs(step) < 1000000.0) break;
        double nx = (double)xi + step;
        if (nx < 10) nx = 10;
        xi = (uint64_t)nx;
    }
    /* walk from xi with known count c = cnt(xi) */
    uint64_t rb = rk_isqrt(2 * xi + 100) + 2;
    bool *bc = (bool *)calloc(rb + 1, sizeof(bool));
    uint32_t *bp = (uint32_t *)malloc((rb + 1) * sizeof(uint32_t));
    bool *seg = (bool *)malloc(RK_BL);
    if (!bc || !bp || !seg) abort();
    size_t np = 0;
    for (uint64_t i = 2; i <= rb; ++i) {
        if (!bc[i]) {
            bp[np++] = (uint32_t)i;
            for (uint64_t j = i * i; j <= rb; j += i) bc[j] = true;
        }
    }
    uint64_t result = 0;
    if (c >= k) {
        uint64_t hiv = xi;
        for (;;) {
            uint64_t lov = (hiv >= RK_BL) ? hiv - RK_BL + 1 : 0;
            uint64_t len = hiv - lov + 1;
            for (uint64_t i = 0; i < len; ++i) seg[i] = true;
            for (size_t t = 0; t < np; ++t) {
                uint64_t p = bp[t];
                if (p * p > hiv) break;
                uint64_t s = ((lov + p - 1) / p) * p;
                if (s < p * p) s = p * p;
                for (uint64_t v = s; v <= hiv; v += p) seg[v - lov] = false;
            }
            for (uint64_t v = hiv + 1; v-- > lov; ) {
                bool pr = seg[v - lov] && v >= 2;
                bool item = want_prime ? pr : (!pr && v >= 4);
                if (item) {
                    if (c == k) { result = v; goto done; }
                    --c;
                }
            }
            if (lov == 0) break;
            hiv = lov - 1;
        }
    } else {
        uint64_t lov = xi + 1;
        for (;;) {
            uint64_t hiv = lov + RK_BL - 1;
            uint64_t len = hiv - lov + 1;
            for (uint64_t i = 0; i < len; ++i) seg[i] = true;
            for (size_t t = 0; t < np; ++t) {
                uint64_t p = bp[t];
                if (p * p > hiv) break;
                uint64_t s = ((lov + p - 1) / p) * p;
                if (s < p * p) s = p * p;
                for (uint64_t v = s; v <= hiv; v += p) seg[v - lov] = false;
            }
            for (uint64_t v = lov; v <= hiv; ++v) {
                bool pr = seg[v - lov] && v >= 2;
                bool item = want_prime ? pr : (!pr && v >= 4);
                if (item) {
                    ++c;
                    if (c == k) { result = v; goto done; }
                }
            }
            lov = hiv + 1;
        }
    }
done:
    free(bc); free(bp); free(seg);
    return result;
}

static uint64_t rk_A073846(uint64_t m)
{
    if (m == 1) return 1;
    uint64_t k = m / 2;
    if (k == 0) return 0;
    return rk_find(k, (m & 1) == 0);
}

static uint64_t *rk_cache = NULL;
static size_t rk_have = 0, rk_cap = 0;

seq_t a(idx_t n)
{
    if (rk_cap == 0) {
        rk_cap = 256;
        rk_cache = (uint64_t *)malloc(rk_cap * sizeof(uint64_t));
        if (!rk_cache) abort();
        rk_cache[0] = 50;
        rk_have = 1;
    }
    while (rk_have <= (size_t)n) {
        if (rk_have >= rk_cap) {
            rk_cap *= 2;
            rk_cache = (uint64_t *)realloc(rk_cache, rk_cap * sizeof(uint64_t));
            if (!rk_cache) abort();
        }
        rk_cache[rk_have] = rk_A073846(rk_cache[rk_have - 1]);
        ++rk_have;
    }
    return (seq_t)rk_cache[n];
}
