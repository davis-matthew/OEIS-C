/*
 * A390737  Number of decimal places which are known after using n terms of
 *          A084580 and treating them as the coefficients of a continued
 *          fraction.
 * https://oeis.org/A390737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of leading characters (integer digit 0 plus
 *            decimals) on which [0; A084580(1..n)] and [0; A084580(1..n), 1]
 *            agree in decimal; computed with exact big-integer convergents
 *            and long division
 * Range:     u8 n <= 305, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 85 terms through n = 4616 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "../utils/mem.h"

static double A084580_gauss_kuzmin(idx_t k)
{
    return log1p(1.0 / ((double)k * ((double)k + 2.0))) / log(2.0);
}

static seq_t *A084580_cache = NULL;
static size_t A084580_cache_len = 0;
static size_t A084580_cache_cap = 0;
static uint32_t *A084580_cnt = NULL;
static double   *A084580_p   = NULL;
static size_t A084580_k_cap  = 0;

static void A084580_ensure_k_capacity(idx_t min_cap)
{
    if (A084580_k_cap >= (size_t)min_cap) return;
    size_t new_cap = A084580_k_cap == 0 ? 1 : A084580_k_cap;
    while (new_cap < (size_t)min_cap) new_cap <<= 1;
    uint32_t *new_cnt = (uint32_t *)realloc(A084580_cnt, (new_cap + 1) * sizeof(uint32_t));
    double   *new_p   = (double   *)realloc(A084580_p,   (new_cap + 1) * sizeof(double));
    if (!new_cnt || !new_p) return;
    for (size_t i = A084580_k_cap + 1; i <= new_cap; ++i) {
        new_cnt[i] = 0;
        new_p[i]   = 0.0;
    }
    A084580_cnt = new_cnt;
    A084580_p   = new_p;
    A084580_k_cap = new_cap;
}

static void A084580_ensure_cache(idx_t n)
{
    if (A084580_cache_len >= (size_t)n) return;
    size_t new_cap = A084580_cache_cap == 0 ? 1 : A084580_cache_cap;
    while (new_cap < (size_t)n) new_cap <<= 1;
    seq_t *new_cache = mem_resize(seq_t, A084580_cache, new_cap);
    if (!new_cache) return;
    A084580_cache = new_cache;
    A084580_cache_cap = new_cap;
}

static seq_t A084580_a(idx_t n)
{
    if (n == 0) return 0;
    A084580_ensure_cache(n);
    A084580_ensure_k_capacity(n);
    while (A084580_cache_len < (size_t)n) {
        uint32_t best_k = 1;
        double best_r = (A084580_cnt[1] + 1.0) / (A084580_p[1] == 0.0 ? A084580_gauss_kuzmin(1) : A084580_p[1]);
        for (uint32_t k = 2; k <= (uint32_t)n; ++k) {
            if (A084580_p[k] == 0.0) A084580_p[k] = A084580_gauss_kuzmin(k);
            double r = (A084580_cnt[k] + 1.0) / A084580_p[k];
            if (r < best_r) {
                best_r = r;
                best_k = k;
            }
        }
        A084580_cache[A084580_cache_len++] = (seq_t)best_k;
        A084580_cnt[best_k]++;
    }
    return A084580_cache[n - 1];
}

static int cf_cmp(const uint32_t *a, const uint32_t *b, size_t L)
{
    for (size_t i = L; i-- > 0;) {
        if (a[i] != b[i]) return a[i] > b[i] ? 1 : -1;
    }
    return 0;
}

static void cf_sub(uint32_t *a, const uint32_t *b, size_t L)
{
    uint64_t borrow = 0;
    for (size_t i = 0; i < L; i++) {
        uint64_t s = (uint64_t)b[i] + borrow;
        if ((uint64_t)a[i] >= s) { a[i] = (uint32_t)((uint64_t)a[i] - s); borrow = 0; }
        else { a[i] = (uint32_t)(((uint64_t)1 << 32) + (uint64_t)a[i] - s); borrow = 1; }
    }
}

static void cf_mul10(uint32_t *a, size_t L)
{
    uint64_t c = 0;
    for (size_t i = 0; i < L; i++) {
        uint64_t v = (uint64_t)a[i] * 10u + c;
        a[i] = (uint32_t)v;
        c = v >> 32;
    }
}

static void cf_add(uint32_t *r, const uint32_t *a, const uint32_t *b, size_t L)
{
    uint64_t c = 0;
    for (size_t i = 0; i < L; i++) {
        uint64_t v = (uint64_t)a[i] + b[i] + c;
        r[i] = (uint32_t)v;
        c = v >> 32;
    }
}

/* pa = a*pb + pa (in place), small multiplier a */
static void cf_step(uint32_t *pa, const uint32_t *pb, uint64_t a, size_t L)
{
    uint64_t c = 0;
    for (size_t i = 0; i < L; i++) {
        uint64_t v = a * (uint64_t)pb[i] + (uint64_t)pa[i] + c;
        pa[i] = (uint32_t)v;
        c = v >> 32;
    }
}

seq_t a(idx_t n)
{
    size_t cnt = (size_t)n;
    uint32_t *terms = (uint32_t *)malloc((cnt + 1) * sizeof(uint32_t));
    size_t bits = 8;
    if (cnt > 0) {
        (void)A084580_a(n);
        for (size_t k = 1; k <= cnt; k++) {
            uint32_t t = (uint32_t)(long long)A084580_a((idx_t)k);
            terms[k] = t;
            uint32_t v = t + 1;
            while (v) { bits++; v >>= 1; }
        }
    }
    size_t L = bits / 32 + 4;
    uint32_t *buf = (uint32_t *)calloc(L * 10, sizeof(uint32_t));
    uint32_t *pa = buf, *pb = buf + L, *qa = buf + 2 * L, *qb = buf + 3 * L;
    uint32_t *P2 = buf + 4 * L, *Q2 = buf + 5 * L, *r1 = buf + 6 * L, *r2 = buf + 7 * L;
    pa[0] = 1;       /* p_{-1} */
    pb[0] = 0;       /* p_0 */
    qa[0] = 0;       /* q_{-1} */
    qb[0] = 1;       /* q_0 */
    for (size_t k = 1; k <= cnt; k++) {
        cf_step(pa, pb, terms[k], L);
        uint32_t *t = pa; pa = pb; pb = t;
        cf_step(qa, qb, terms[k], L);
        t = qa; qa = qb; qb = t;
    }
    /* x = pb/qb, y = (pb+pa)/(qb+qa) */
    cf_add(P2, pb, pa, L);
    cf_add(Q2, qb, qa, L);
    memcpy(r1, pb, L * sizeof(uint32_t));
    memcpy(r2, P2, L * sizeof(uint32_t));
    int i1 = cf_cmp(r1, qb, L) >= 0;
    int i2 = cf_cmp(r2, Q2, L) >= 0;
    size_t res = 0;
    if (i1 == i2) {
        res = 1;
        if (i1) cf_sub(r1, qb, L);
        if (i2) cf_sub(r2, Q2, L);
        for (;;) {
            cf_mul10(r1, L);
            cf_mul10(r2, L);
            int d1 = 0, d2 = 0;
            while (cf_cmp(r1, qb, L) >= 0) { cf_sub(r1, qb, L); d1++; }
            while (cf_cmp(r2, Q2, L) >= 0) { cf_sub(r2, Q2, L); d2++; }
            if (d1 != d2) break;
            res++;
        }
    }
    free(buf);
    free(terms);
    return (seq_t)res;
}
