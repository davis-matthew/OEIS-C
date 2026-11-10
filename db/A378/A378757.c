/*
 * A378757  Dirichlet convolution of sigma and the Dirichlet inverse of
 *          A005187 (A346237).
 * https://oeis.org/A378757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} A000203(d)*A346237(n/d), where A346237 is the
 *            Dirichlet inverse of A005187 (h(1)=1, h(m) = -Sum_{d|m, d>1}
 *            A005187(d)*h(m/d))
 * Range:     i8 n <= 109, i16 n <= 2729, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 99 terms through n = 9833 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t A005187_a(uint64_t n) {
    seq_t result = 0;
    seq_t power = (seq_t)n;
    while (power > 0) {
        result += power;
        power >>= 1;
    }
    return result;
}

static seq_t A000203_a(uint64_t n) {
    seq_t result = 1;
    uint64_t m = n;
    uint64_t p;
    for (p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            seq_t term = 1;
            seq_t pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)p;
                term += pw;
            }
            result *= term;
        }
    }
    if (m > 1) {
        result *= ((seq_t)m + 1);
    }
    return result;
}

static int cmp_u64_a346237(const void *x, const void *y) {
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return (a > b) - (a < b);
}

static size_t find_div_a346237(const uint64_t *d, size_t cnt, uint64_t v) {
    size_t lo = 0, hi = cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (d[mid] < v) lo = mid + 1; else hi = mid;
    }
    return lo;
}

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    size_t cap = 16, cnt = 0, i, j;
    uint64_t *divs = (uint64_t *)malloc(cap * sizeof(uint64_t));
    uint64_t d;
    for (d = 1; d * d <= N; d++) {
        if (N % d == 0) {
            if (cnt + 2 > cap) {
                cap *= 2;
                divs = (uint64_t *)realloc(divs, cap * sizeof(uint64_t));
            }
            divs[cnt++] = d;
            if (d * d != N) divs[cnt++] = N / d;
        }
    }
    qsort(divs, cnt, sizeof(uint64_t), cmp_u64_a346237);

    seq_t *h = mem_new(seq_t, cnt);
    seq_t *g = mem_new(seq_t, cnt);
    for (i = 0; i < cnt; i++) g[i] = A005187_a(divs[i]);

    h[0] = 1;
    for (i = 1; i < cnt; i++) {
        seq_t s = 0;
        for (j = 1; j <= i; j++) {
            if (divs[i] % divs[j] == 0) {
                size_t k = find_div_a346237(divs, cnt, divs[i] / divs[j]);
                s += g[j] * h[k];
            }
        }
        h[i] = -s;
    }

    seq_t res = 0;
    for (i = 0; i < cnt; i++) {
        size_t k = find_div_a346237(divs, cnt, N / divs[i]);
        res += A000203_a(divs[i]) * h[k];
    }

    mem_free(h);
    mem_free(g);
    free(divs);
    return res;
}
