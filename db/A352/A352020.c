/*
 * A352020  G.f. A(x) satisfies: [x^n] A(x)^(2*n+1) = [x^n] 2*A(x)^(n+1), for
 *          n > 1.
 * https://oeis.org/A352020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 1; for n > 1, [x^n] A(x)^(2n+1) = 2*[x^n]
 *            A(x)^(n+1). Since [x^n] A^m = m*a(n) + (terms in a(1..n-1)),
 *            this gives a(n) = R(2n+1) - 2*R(n+1), where R(m) = [x^n] A(x)^m
 *            computed with a(n) set to 0. Powers are computed by truncated
 *            polynomial multiplication only (no division), so wr...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *cache_a = 0;
static size_t cache_cnt = 0;
static size_t cache_cap = 0;

/* r = x*y truncated to degree k; r may alias x or y */
static void polmul(seq_t *r, const seq_t *x, const seq_t *y, size_t k, seq_t *tmp) {
    for (size_t i = 0; i <= k; i++) {
        seq_t s = 0;
        for (size_t j = 0; j <= i; j++) s += x[j] * y[i - j];
        tmp[i] = s;
    }
    for (size_t i = 0; i <= k; i++) r[i] = tmp[i];
}

/* coefficient of x^k in A^m, A[0..k] */
static seq_t pow_coef(size_t m, size_t k, const seq_t *A) {
    seq_t *res = mem_new(seq_t, k + 1);
    seq_t *base = mem_new(seq_t, k + 1);
    seq_t *tmp = mem_new(seq_t, k + 1);
    for (size_t i = 0; i <= k; i++) { res[i] = 0; base[i] = A[i]; }
    res[0] = 1;
    size_t e = m;
    while (e > 0) {
        if (e & 1) polmul(res, res, base, k, tmp);
        e >>= 1;
        if (e > 0) polmul(base, base, base, k, tmp);
    }
    seq_t r = res[k];
    mem_free(res);
    mem_free(base);
    mem_free(tmp);
    return r;
}

seq_t a(idx_t n) {
    if (cache_cap == 0) {
        cache_cap = 64;
        cache_a = mem_new(seq_t, cache_cap);
        for (size_t i = 0; i < cache_cap; i++) cache_a[i] = 0;
        cache_a[0] = 1;
        cache_a[1] = 1;
        cache_cnt = 2;
    }
    if ((size_t)n >= cache_cap) {
        size_t nc = cache_cap;
        while (nc <= (size_t)n) nc *= 2;
        cache_a = mem_resize(seq_t, cache_a, nc);
        for (size_t i = cache_cap; i < nc; i++) cache_a[i] = 0;
        cache_cap = nc;
    }
    while (cache_cnt <= (size_t)n) {
        size_t k = cache_cnt;
        cache_a[k] = 0;
        seq_t r1 = pow_coef(2 * k + 1, k, cache_a);
        seq_t r2 = pow_coef(k + 1, k, cache_a);
        cache_a[k] = r1 - 2 * r2;
        cache_cnt++;
    }
    return cache_a[n];
}
