/*
 * A331751  Numbers k such that A048675(sigma(k)) is equal to A048675(2*k).
 * https://oeis.org/A331751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search, not a closed formula. k is a term iff A048675(sigma(k))
 *            = A048675(2k) = 1 + A048675(k). A segmented sieve factors each
 *            k. Because A048675 is completely additive, A048675(sigma(k)) mod
 *            2^64 is the sum over p^e || k of A048675(sigma(p^e)), and both
 *            sides are compared modulo 2^64 first. Cand...
 * Range:     u8 n <= 5, u16 n <= 18, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: standard, pass, 59 terms through n = 60 (2026-11-08)
 * Helpers:   factor_u64, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime_u64.h"

#define A331751_B 65536u
#define A331751_MAXT 48

static uint64_t *A331751_sp = NULL;     /* primes */
static uint64_t *A331751_inv = NULL;    /* inverse mod 2^64 (odd primes) */
static uint64_t *A331751_lim = NULL;    /* UINT64_MAX / p */
static uint64_t *A331751_Ls = NULL;     /* L(sigma(p^t)) mod 2^64 */
static unsigned char *A331751_dc = NULL;
static size_t A331751_spn = 0;
static uint64_t A331751_splim = 0;

static void A331751_ensure(uint64_t lim)
{
    if (lim <= A331751_splim) return;
    uint64_t nl = lim * 2;
    if (nl < 1000) nl = 1000;
    unsigned char *c = (unsigned char *)calloc((size_t)nl + 1, 1);
    size_t cap = (size_t)nl / 2 + 16;
    uint64_t *p = (uint64_t *)malloc(sizeof(uint64_t) * cap);
    size_t cnt = 0;
    for (uint64_t i = 2; i <= nl; ++i) {
        if (c[i]) continue;
        p[cnt++] = i;
        for (uint64_t j = i * i; j <= nl; j += i) c[j] = 1;
    }
    free(c);
    if (A331751_sp) free(A331751_sp);
    if (A331751_inv) free(A331751_inv);
    if (A331751_lim) free(A331751_lim);
    if (A331751_Ls) free(A331751_Ls);
    if (A331751_dc) free(A331751_dc);
    A331751_sp = p;
    A331751_spn = cnt;
    A331751_inv = (uint64_t *)malloc(sizeof(uint64_t) * cnt);
    A331751_lim = (uint64_t *)malloc(sizeof(uint64_t) * cnt);
    for (size_t j = 0; j < cnt; ++j) {
        uint64_t q = p[j], x = q;
        if (q & 1) for (int it = 0; it < 6; ++it) x *= 2 - q * x;
        A331751_inv[j] = x;
        A331751_lim[j] = UINT64_MAX / q;
    }
    A331751_Ls = (uint64_t *)calloc(cnt * A331751_MAXT, sizeof(uint64_t));
    A331751_dc = (unsigned char *)calloc(cnt, 1);
    A331751_splim = nl;
}

/* A048675(m) mod 2^64 (only the first 64 primes matter) */
static uint64_t A331751_L(uint64_t m)
{
    if (m == 0) return 0;
    uint64_t e = (uint64_t)__builtin_ctzll(m);
    m >>= e;
    uint64_t acc = e;
    for (int j = 1; j < 64 && m > 1; ++j) {
        uint64_t iv = A331751_inv[j], lm = A331751_lim[j];
        e = 0;
        while (m * iv <= lm) { m *= iv; ++e; }
        acc += e << j;
    }
    return acc;
}

static uint64_t A331751_Lget(size_t j, int t)
{
    if (t == 0) return 0;
    while ((int)A331751_dc[j] < t) {
        int tt = A331751_dc[j] + 1;
        uint64_t p = A331751_sp[j], pw = 1, s = 1;
        for (int i = 0; i < tt; ++i) { pw *= p; s += pw; }
        A331751_Ls[j * A331751_MAXT + tt] = A331751_L(s);
        A331751_dc[j] = (unsigned char)tt;
    }
    return A331751_Ls[j * A331751_MAXT + t];
}

static uint64_t A331751_nextp(uint64_t p)
{
    uint64_t c = p + 1;
    while (!is_prime_u64(c)) ++c;
    return c;
}

typedef struct { uint64_t pos[400]; uint64_t co[400]; int n; } A331751_acc;

static void A331751_add(A331751_acc *a, uint64_t pos, uint64_t c)
{
    int i = 0;
    while (i < a->n && a->pos[i] < pos) ++i;
    if (i < a->n && a->pos[i] == pos) { a->co[i] += c; return; }
    for (int j = a->n; j > i; --j) { a->pos[j] = a->pos[j-1]; a->co[j] = a->co[j-1]; }
    a->pos[i] = pos; a->co[i] = c; a->n++;
}

static void A331751_build(uint64_t m, A331751_acc *a)
{
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    a->n = 0;
    if (m > 1) factor_u64(m, pr, ex, &cnt);
    for (size_t i = 0; i < cnt; ++i) A331751_add(a, pr[i], ex[i]);
    for (int i = 0; i < a->n; ++i) {
        if (a->co[i] >= 2) {
            uint64_t c = a->co[i] >> 1;
            a->co[i] &= 1;
            A331751_add(a, A331751_nextp(a->pos[i]), c);
        }
    }
}

static bool A331751_exact(uint64_t k)
{
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    uint64_t s = 1;
    if (k > 1) factor_u64(k, pr, ex, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t pw = 1, t = 1;
        for (uint64_t e = 0; e < ex[i]; ++e) { pw *= pr[i]; t += pw; }
        s *= t;
    }
    A331751_acc x, y;
    A331751_build(2 * k, &x);
    A331751_build(s, &y);
    int i = 0, j = 0;
    for (;;) {
        while (i < x.n && x.co[i] == 0) ++i;
        while (j < y.n && y.co[j] == 0) ++j;
        if (i >= x.n || j >= y.n) break;
        if (x.pos[i] != y.pos[j]) return false;
        ++i; ++j;
    }
    return i >= x.n && j >= y.n;
}

static uint64_t *A331751_terms = NULL;
static size_t A331751_cnt = 0, A331751_cap = 0;
static uint64_t A331751_next = 1;

static void A331751_scan_block(void)
{
    static uint64_t rem[A331751_B], lowk[A331751_B], sl[A331751_B];
    uint64_t lo = A331751_next, hi = lo + A331751_B; /* [lo, hi) */
    uint64_t sq = 1;
    while (sq * sq < hi) ++sq;
    A331751_ensure(sq + 2);
    for (uint32_t i = 0; i < A331751_B; ++i) { rem[i] = lo + i; lowk[i] = 0; sl[i] = 0; }
    for (size_t j = 0; j < A331751_spn; ++j) {
        uint64_t p = A331751_sp[j];
        if (p * p >= hi) break;
        uint64_t bit = (j < 64) ? ((uint64_t)1 << j) : 0;
        uint64_t iv = A331751_inv[j];
        uint64_t q = p;
        int t = 1;
        for (;;) {
            uint64_t d = A331751_Lget(j, t) - A331751_Lget(j, t - 1);
            uint64_t st = ((lo + q - 1) / q) * q;
            if (p == 2) {
                for (uint64_t m = st; m < hi; m += q) {
                    uint32_t i = (uint32_t)(m - lo);
                    rem[i] >>= 1; lowk[i] += bit; sl[i] += d;
                }
            } else {
                for (uint64_t m = st; m < hi; m += q) {
                    uint32_t i = (uint32_t)(m - lo);
                    rem[i] *= iv; lowk[i] += bit; sl[i] += d;
                }
            }
            if (q > (hi - 1) / p) break;
            q *= p;
            ++t;
            if (t >= A331751_MAXT) break;
        }
    }
    for (uint32_t i = 0; i < A331751_B; ++i) {
        uint64_t k = lo + i, r = rem[i], lw = lowk[i], s = sl[i];
        bool cand = false;
        if (r == 1) {
            cand = (s == lw + 1);
        } else {
            if (r <= 311) {
                for (int j = 0; j < 64; ++j)
                    if (A331751_sp[j] == r) { lw += (uint64_t)1 << j; break; }
            }
            uint64_t need = lw + 1 - s;
            uint64_t u = r + 1;
            uint64_t e = (uint64_t)__builtin_ctzll(u);
            u >>= e;
            uint64_t acc = e;
            if ((acc ^ need) & 1) continue;
            bool ok = true;
            for (int j = 1; j < 64 && u > 1; ++j) {
                uint64_t ivj = A331751_inv[j], lm = A331751_lim[j];
                e = 0;
                while (u * ivj <= lm) { u *= ivj; ++e; }
                acc += e << j;
                uint64_t mask = (j == 63) ? ~(uint64_t)0 : (((uint64_t)2 << j) - 1);
                if ((acc ^ need) & mask) { ok = false; break; }
            }
            cand = ok && (acc == need);
        }
        if (cand && A331751_exact(k)) {
            if (A331751_cnt == A331751_cap) {
                A331751_cap = A331751_cap ? A331751_cap * 2 : 128;
                A331751_terms = (uint64_t *)realloc(A331751_terms, A331751_cap * sizeof(uint64_t));
            }
            A331751_terms[A331751_cnt++] = k;
        }
    }
    A331751_next = hi;
}

seq_t a(idx_t n)
{
    A331751_ensure(400);
    while (A331751_cnt < (size_t)n) A331751_scan_block();
    return (seq_t)A331751_terms[n - 1];
}
