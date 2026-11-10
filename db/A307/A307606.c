/*
 * A307606  G.f. A(x) satisfies: A(x) = ((1 + x)/(1 - x)) *
 *          A(x^2)^2*A(x^3)^3*A(x^4)^4* ... *A(x^k)^k* ...
 * https://oeis.org/A307606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. Product_{k>=1} ((1 + x^k)/(1 - x^k))^(k*A074206(k)) =
 *            Product_{m>=1} (1 - x^m)^(-c(m)), c(m) = 2*m*A074206(m) - [m
 *            even]*(m/2)*A074206(m/2). Arbitrary precision: a(0)=1, a(n) =
 *            (1/n)*Sum_{j=1..n} b(j)*a(n-j), b(j) = Sum_{d|j} d*c(d). Fixed
 *            width: the product is expanded directly, with no div...
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 30, u64 n <= 72, u128 n <= 178,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 4879 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A307606_cache = NULL;
static idx_t A307606_len = 0;
static int A307606_width = -1;   /* bit width of seq_t, 300 = unbounded */

static int A307606_get_width(void)
{
    seq_t x = (seq_t)1;
    int w = 0;
    while (x != (seq_t)0 && w < 300) {
        x = x + x;
        ++w;
    }
    return w;
}

/* exact path (big integers): logarithmic-derivative recurrence */
static void A307606_build_exact(idx_t N)
{
    idx_t i, j, d;
    if (N < 1) N = 1;
    seq_t *f = mem_new(seq_t, N + 1);
    seq_t *c = mem_new(seq_t, N + 1);
    seq_t *b = mem_new(seq_t, N + 1);
    seq_t *r = mem_new(seq_t, N + 1);

    f[1] = (seq_t)1;
    for (d = 1; d <= N; ++d)
        for (j = 2 * d; j <= N; j += d)
            f[j] += f[d];
    for (i = 1; i <= N; ++i) {
        seq_t v = (seq_t)2 * (seq_t)i * f[i];
        if ((i & 1) == 0)
            v -= (seq_t)(i / 2) * f[i / 2];
        c[i] = v;
    }
    for (d = 1; d <= N; ++d) {
        seq_t t = (seq_t)d * c[d];
        for (j = d; j <= N; j += d)
            b[j] += t;
    }
    r[0] = (seq_t)1;
    for (i = 1; i <= N; ++i) {
        seq_t s = (seq_t)0;
        for (j = 1; j <= i; ++j)
            s += b[j] * r[i - j];
        r[i] = s / (seq_t)i;
    }
    mem_free(f);
    mem_free(c);
    mem_free(b);
    if (A307606_cache)
        mem_free(A307606_cache);
    A307606_cache = r;
    A307606_len = N + 1;
}

/* truncated series product: out = x*y mod t^(M+1) */
static void A307606_mul(const seq_t *x, const seq_t *y, seq_t *out, idx_t M)
{
    idx_t i, j;
    for (i = 0; i <= M; ++i) {
        seq_t s = (seq_t)0;
        for (j = 0; j <= i; ++j)
            s += x[j] * y[i - j];
        out[i] = s;
    }
}

/* fixed-width path: direct product, ring operations only (exact mod 2^w) */
static void A307606_build_fixed(idx_t N)
{
    idx_t i, j, m, k;
    if (N < 1) N = 1;
    uint64_t *f = (uint64_t *)calloc((size_t)(N + 1), sizeof(uint64_t));
    seq_t *r = mem_new(seq_t, N + 1);
    seq_t *res = mem_new(seq_t, N + 1);
    seq_t *base = mem_new(seq_t, N + 1);
    seq_t *tmp = mem_new(seq_t, N + 1);

    f[1] = 1;
    for (m = 1; m <= N; ++m)
        for (j = 2 * m; j <= N; j += m)
            f[j] += f[m];

    r[0] = (seq_t)1;
    for (m = 1; m <= N; ++m) {
        uint64_t e = 2 * (uint64_t)m * f[m];
        if ((m & 1) == 0)
            e -= (uint64_t)(m / 2) * f[m / 2];
        idx_t M = N / m;
        for (i = 0; i <= M; ++i) {
            res[i] = (seq_t)(i == 0 ? 1 : 0);
            base[i] = (seq_t)1;           /* 1/(1-t) */
        }
        while (e) {
            if (e & 1) {
                A307606_mul(res, base, tmp, M);
                for (i = 0; i <= M; ++i) res[i] = tmp[i];
            }
            e >>= 1;
            if (e) {
                A307606_mul(base, base, tmp, M);
                for (i = 0; i <= M; ++i) base[i] = tmp[i];
            }
        }
        /* r <- r * res(x^m), in place from the top */
        for (i = N; i >= m; --i) {
            seq_t s = r[i];
            for (k = 1; k * m <= i; ++k)
                s += res[k] * r[i - k * m];
            r[i] = s;
            if (i == m) break;
        }
    }
    free(f);
    mem_free(res);
    mem_free(base);
    mem_free(tmp);
    if (A307606_cache)
        mem_free(A307606_cache);
    A307606_cache = r;
    A307606_len = N + 1;
}

seq_t a(idx_t n)
{
    if (A307606_width < 0)
        A307606_width = A307606_get_width();

    if (A307606_width < 300) {
        /* a(4w) far exceeds 2^w, so larger terms cannot fit in seq_t */
        idx_t cap = (idx_t)(4 * A307606_width);
        if (A307606_len == 0)
            A307606_build_fixed(cap);
        if (n >= A307606_len)
            return (seq_t)0;
        return A307606_cache[n];
    }

    if (n >= A307606_len) {
        idx_t N = n;
        if (2 * A307606_len > N + 1)
            N = 2 * A307606_len - 1;
        A307606_build_exact(N);
    }
    return A307606_cache[n];
}
