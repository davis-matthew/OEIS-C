/*
 * A308337  Expansion of e.g.f. exp(-1 + Product_{k>=1} 1/(1 - x^k/k)).
 * https://oeis.org/A308337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=1..n}
 *            binomial(n-1,k-1)*A007841(k)*a(n-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 388 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A007841_alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static void A007841_release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

static seq_t *A007841_cache = 0;
static idx_t A007841_cache_len = 0;

static void A007841_build(idx_t N) {
    idx_t len = N + 1;
    seq_t *A = A007841_alloc_zero(len);
    A[0] = 1;
    seq_t fact = 1;
    for (idx_t m = 1; m <= N; m++) {
        seq_t coef = fact;
        for (idx_t k = m; k <= N; k++) {
            if (k > m) {
                coef = coef * (seq_t)k / (seq_t)(k - m);
            }
            A[k] += coef * A[k - m];
        }
        fact *= (seq_t)m;
    }
    if (A007841_cache) A007841_release(A007841_cache, A007841_cache_len);
    A007841_cache = A;
    A007841_cache_len = len;
}

static seq_t A007841_a(idx_t n) {
    if (n >= A007841_cache_len) {
        idx_t N = n;
        if (A007841_cache_len > 0 && N < 2 * (A007841_cache_len - 1)) N = 2 * (A007841_cache_len - 1);
        if (N < 16) N = 16;
        A007841_build(N);
    }
    return A007841_cache[n];
}

static seq_t *A308337_cache = 0;
static idx_t A308337_len = 0;

static void A308337_build(idx_t N) {
    seq_t *r = mem_new(seq_t, (size_t)(N + 1));
    seq_t *b = mem_new(seq_t, (size_t)(N + 1));
    for (idx_t k = 0; k <= N; k++) b[k] = A007841_a(k);
    r[0] = 1;
    for (idx_t n = 1; n <= N; n++) {
        seq_t s = 0;
        seq_t c = 1; /* binomial(n-1, k-1) */
        for (idx_t k = 1; k <= n; k++) {
            s += c * b[k] * r[n - k];
            if (k < n) c = c * (seq_t)(n - k) / (seq_t)k;
        }
        r[n] = s;
    }
    mem_free(b);
    if (A308337_cache) mem_free(A308337_cache);
    A308337_cache = r;
    A308337_len = N + 1;
}

seq_t a(idx_t n) {
    if (n >= A308337_len) {
        idx_t N = n;
        if (N < 32) N = 32;
        A308337_build(N);
    }
    return A308337_cache[n];
}
