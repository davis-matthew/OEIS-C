/*
 * A349435  Dirichlet convolution of A230593 with A347084, which is Dirichlet
 *          inverse of {n + its arithmetic derivative}.
 * https://oeis.org/A349435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} A230593(n/d) * A347084(d), where A230593(m) = m
 *            + Sum_{p|m} m/p and A347084 is the Dirichlet inverse of
 *            A129283(m) = m + m'. Computed over the divisors of n: g(1) = 1,
 *            g(d) = -Sum_{e|d, e>1} A129283(e)*g(d/e)
 * Range:     i8 n <= 2196, i16 n <= 20000, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 115 terms through n = 9881 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static int cmp_u64_a349435(const void *x, const void *y) {
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return (a > b) - (a < b);
}

static seq_t A129283_a349435(uint64_t e, const uint64_t *pr, size_t c) {
    seq_t der = 0;
    for (size_t i = 0; i < c; ++i) {
        uint64_t t = e;
        uint64_t k = 0;
        while (t % pr[i] == 0) { t /= pr[i]; ++k; }
        if (k) der += (seq_t)k * (seq_t)(e / pr[i]);
    }
    return (seq_t)e + der;
}

static seq_t A230593_a349435(uint64_t m, const uint64_t *pr, size_t c) {
    seq_t s = (seq_t)m;
    for (size_t i = 0; i < c; ++i) {
        if (m % pr[i] == 0) s += (seq_t)(m / pr[i]);
    }
    return s;
}

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    if (N == 0) return 0;
    uint64_t pr[24], ex[24];
    size_t c = 0;
    uint64_t t = N;
    for (uint64_t p = 2; p * p <= t; p += (p == 2 ? 1 : 2)) {
        if (t % p == 0) {
            uint64_t e = 0;
            while (t % p == 0) { t /= p; ++e; }
            pr[c] = p; ex[c++] = e;
        }
    }
    if (t > 1) { pr[c] = t; ex[c++] = 1; }

    size_t nd = 1;
    for (size_t i = 0; i < c; ++i) nd *= (size_t)(ex[i] + 1);
    uint64_t *d = (uint64_t *)malloc(nd * sizeof(uint64_t));
    size_t cnt = 1;
    d[0] = 1;
    for (size_t i = 0; i < c; ++i) {
        size_t base = cnt;
        uint64_t pw = 1;
        for (uint64_t k = 1; k <= ex[i]; ++k) {
            pw *= pr[i];
            for (size_t j = 0; j < base; ++j) d[cnt++] = d[j] * pw;
        }
    }
    qsort(d, cnt, sizeof(uint64_t), cmp_u64_a349435);

    seq_t *f = mem_new(seq_t, cnt);
    seq_t *g = mem_new(seq_t, cnt);
    for (size_t i = 0; i < cnt; ++i) f[i] = A129283_a349435(d[i], pr, c);
    g[0] = 1;
    for (size_t i = 1; i < cnt; ++i) {
        seq_t sum = 0;
        for (size_t j = 1; j <= i; ++j) {
            if (d[i] % d[j] == 0) {
                uint64_t q = d[i] / d[j];
                size_t lo = 0, hi = i;
                while (lo < hi) {
                    size_t mid = (lo + hi) / 2;
                    if (d[mid] < q) lo = mid + 1; else hi = mid;
                }
                sum += f[j] * g[lo];
            }
        }
        g[i] = -sum;
    }
    seq_t res = 0;
    for (size_t i = 0; i < cnt; ++i) {
        res += A230593_a349435(N / d[i], pr, c) * g[i];
    }
    mem_free(f);
    mem_free(g);
    free(d);
    return res;
}
