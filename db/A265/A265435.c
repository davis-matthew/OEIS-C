/*
 * A265435  Riordan array (1, x*f(x)) where f(x) is the g.f. of A007564.
 * https://oeis.org/A265435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = [x^(n-k)] f(x)^k where f(x) = Sum_{i>=0} A007564(i)
 *            x^i; a(n) = T(r,c) read by rows (0<=c<=r)
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 120, u64 n <= 378,
 *            u128 n <= 1225, big any n
 * Validated: standard, pass, 82 terms through n = 9107 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A007564_cache = NULL;
static idx_t A007564_cache_len = 0;
static idx_t A007564_max_computed = 1;

static seq_t A007564_a(idx_t n) {
    if (n == 0 || n == 1) return 1;

    if (A007564_cache == NULL) {
        A007564_cache_len = (n + 10 < 100) ? 100 : n + 10;
        A007564_cache = mem_new(seq_t, A007564_cache_len);
        A007564_cache[0] = 1;
        A007564_cache[1] = 1;
    } else if (A007564_cache_len <= n) {
        idx_t newlen = A007564_cache_len;
        while (newlen <= n) newlen *= 2;
        seq_t *nc = mem_new(seq_t, newlen);
        for (idx_t j = 0; j <= A007564_max_computed; j++) nc[j] = A007564_cache[j];
        mem_free(A007564_cache);
        A007564_cache = nc;
        A007564_cache_len = newlen;
    }

    for (idx_t i = A007564_max_computed + 1; i <= n; i++) {
        idx_t d = i + 1;
        seq_t D = (seq_t)d;
        if (D == 0) { A007564_cache[i] = 0; continue; }
        seq_t a1 = A007564_cache[i - 1];
        seq_t a2 = A007564_cache[i - 2];
        seq_t q1 = a1 / D, q2 = a2 / D;
        long long r1 = (long long)(a1 % D);
        long long r2 = (long long)(a2 % D);
        long long R = 4LL * (long long)(2 * i - 1) * r1 - 4LL * (long long)(i - 2) * r2;
        seq_t val = (seq_t)4 * (seq_t)(2 * i - 1) * q1;
        if (R >= 0) val += (seq_t)(idx_t)(R / (long long)d);
        val -= (seq_t)4 * (seq_t)(i - 2) * q2;
        if (R < 0) val -= (seq_t)(idx_t)((-R) / (long long)d);
        A007564_cache[i] = val;
    }
    if (n > A007564_max_computed) A007564_max_computed = n;

    return A007564_cache[n];
}

seq_t a(idx_t n) {
    idx_t r = 0, cum = 0;
    while (cum + (r + 1) <= n) {
        cum += r + 1;
        ++r;
    }
    idx_t k = n - cum;
    idx_t d = r - k + 1; /* number of coefficients needed */

    seq_t *f = mem_new(seq_t, d);
    seq_t *p = mem_new(seq_t, d);
    seq_t *q = mem_new(seq_t, d);
    for (idx_t i = 0; i < d; i++) f[i] = A007564_a(i);
    p[0] = 1;

    for (idx_t j = 0; j < k; j++) {
        for (idx_t i = 0; i < d; i++) {
            seq_t s = 0;
            for (idx_t t = 0; t <= i; t++) s += p[t] * f[i - t];
            q[i] = s;
        }
        seq_t *tmp = p; p = q; q = tmp;
    }

    seq_t res = p[d - 1];
    mem_free(f);
    mem_free(p);
    mem_free(q);
    return res;
}
