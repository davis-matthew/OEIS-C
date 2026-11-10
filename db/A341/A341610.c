/*
 * A341610  Nonprimitive terms of A246282: numbers k that have more than one
 *          divisor d|k such that A003961(d) > 2*d.
 * https://oeis.org/A341610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that #{d | k : A003961(d) > 2*d} > 1
 *            (A337345(k) > 1); found by incremental search with memoization
 * Range:     u8 n <= 116, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9973 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A341610_next_prime(uint64_t p) {
    uint64_t q = p + 1;
    for (;;) {
        bool ok = true;
        for (uint64_t i = 2; i * i <= q; i++) {
            if (q % i == 0) { ok = false; break; }
        }
        if (ok) return q;
        q++;
    }
}

static bool A341610_test(uint64_t k) {
    uint64_t pr[24], ex[24];
    int np = 0;
    uint64_t m = k;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            pr[np] = p; ex[np] = 0;
            while (m % p == 0) { m /= p; ex[np]++; }
            np++;
        }
    }
    if (m > 1) { pr[np] = m; ex[np] = 1; np++; }

    size_t cap = 1;
    for (int i = 0; i < np; i++) cap *= (size_t)(ex[i] + 1);
    uint64_t *d = (uint64_t *)malloc(cap * sizeof(uint64_t));
    uint64_t *av = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!d || !av) { free(d); free(av); return false; }
    size_t len = 1;
    d[0] = 1; av[0] = 1;
    for (int i = 0; i < np; i++) {
        uint64_t q = A341610_next_prime(pr[i]);
        size_t base = len;
        uint64_t pd = 1, qa = 1;
        for (uint64_t e = 1; e <= ex[i]; e++) {
            pd *= pr[i];
            qa *= q;
            for (size_t j = 0; j < base; j++) {
                d[len] = d[j] * pd;
                av[len] = av[j] * qa;
                len++;
            }
        }
    }
    int cnt = 0;
    for (size_t j = 0; j < len; j++) {
        if (av[j] > 2 * d[j]) {
            if (++cnt > 1) break;
        }
    }
    free(d);
    free(av);
    return cnt > 1;
}

seq_t a(idx_t n) {
    static uint64_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static uint64_t next_k = 1;

    if (n == 0) return (seq_t)0;
    while (cache_len < (size_t)n) {
        next_k++;
        if (A341610_test(next_k)) {
            if (cache_len >= cache_cap) {
                size_t nc = cache_cap ? cache_cap * 2 : 1024;
                uint64_t *t = (uint64_t *)realloc(cache, nc * sizeof(uint64_t));
                if (!t) return (seq_t)0;
                cache = t;
                cache_cap = nc;
            }
            cache[cache_len++] = next_k;
        }
    }
    return (seq_t)cache[n - 1];
}
