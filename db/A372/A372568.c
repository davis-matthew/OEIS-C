/*
 * A372568  a(n) = Sum_{d|n} A346242(d), where A346242 is the Dirichlet
 *          inverse of gcd(n, A276086(n)).
 * https://oeis.org/A372568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} A346242(d), where A346242 is the Dirichlet
 *            inverse of f(n) = gcd(n, A276086(n)); computed via divisors of n
 *            and the recursion b(m) = -Sum_{e|m, e>1} f(e)*b(m/e)
 * Range:     i8 n <= 89, i16 n <= 8099, i32 n <= 65537, i64 n <= 65537,
 *            i128 n <= 65537, big any n
 * Validated: standard, pass, 114 terms through n = 9942 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* gcd(n, A276086(n)) computed through primorial-base digits, no overflow */
static uint64_t f_gcd_a276086(uint64_t n)
{
    static const uint64_t pr[15] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47};
    uint64_t prim[15];
    int maxk = 0;
    prim[0] = 1;
    for (int k = 1; k < 15; ++k) {
        if (prim[k-1] > UINT64_MAX / pr[k-1]) break;
        prim[k] = prim[k-1] * pr[k-1];
        if (prim[k] > n) break;
        maxk = k;
    }
    uint64_t rem = n, res = 1;
    for (int k = maxk; k >= 0; --k) {
        uint64_t digit = rem / prim[k];
        rem -= digit * prim[k];
        if (digit == 0) continue;
        uint64_t p = pr[k];
        uint64_t t = n, v = 0;
        while (v < digit && t % p == 0) { t /= p; ++v; }
        for (uint64_t i = 0; i < v; ++i) res *= p;
    }
    return res;
}

static int cmp_u64(const void *x, const void *y)
{
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return (a > b) - (a < b);
}

static size_t find_div(const uint64_t *d, size_t cnt, uint64_t v)
{
    size_t lo = 0, hi = cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (d[mid] < v) lo = mid + 1; else hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    size_t cap = 16, cnt = 0;
    uint64_t *d = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!d) abort();
    for (uint64_t i = 1; i * i <= N; ++i) {
        if (N % i) continue;
        if (cnt + 2 > cap) {
            cap *= 2;
            d = (uint64_t *)realloc(d, cap * sizeof(uint64_t));
            if (!d) abort();
        }
        d[cnt++] = i;
        if (i != N / i) d[cnt++] = N / i;
    }
    qsort(d, cnt, sizeof(uint64_t), cmp_u64);

    uint64_t *f = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!f) abort();
    for (size_t i = 0; i < cnt; ++i) f[i] = f_gcd_a276086(d[i]);

    seq_t *b = mem_new(seq_t, cnt);
    seq_t total = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (i == 0) { b[0] = 1; total += b[0]; continue; }
        seq_t s = 0;
        for (size_t j = 0; j < i; ++j) {
            if (d[i] % d[j]) continue;
            size_t q = find_div(d, cnt, d[i] / d[j]);
            s += b[j] * (seq_t)f[q];
        }
        b[i] = -s;
        total += b[i];
    }
    mem_free(b);
    free(f);
    free(d);
    return total;
}
