/*
 * A334448  Decimal expansion of Product_{k>=1} (1 - 1/A002145(k)^4).
 * https://oeis.org/A334448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   digits of Product_{k>=1} (1 - 1/A002145(k)^4) = Prod_{p<=M, p==3
 *            (mod 4)} (1-p^-4) * C(4), C(4) = Prod_{j>=0}
 *            R(4*2^j)^(1/2^(j+1)), with C(s) = Prod_{p>M, p==3 (mod 4)}
 *            (1-p^-s) and R(s) = C(s)^2/C(2s) =
 *            beta(s)*Prod_{2<p<=M}(1-chi_4(p)p^-s) /
 *            (zeta(s)*Prod_{p<=M}(1-p^-s)), zeta(s) = eta(s)/(1-2^...
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 106 terms through n = 105 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define A334448_BASE 1000000000ULL
#define A334448_M 1000

static size_t g_F = 0, g_L = 0, g_N = 0;
static uint32_t *g_W = 0;
static unsigned char *g_dig = 0;
static size_t g_dig_len = 0;

static uint32_t *l_new(size_t n) {
    return (uint32_t *)calloc(n ? n : 1, sizeof(uint32_t));
}
static void l_div(uint32_t *a, size_t n, uint64_t m) {
    uint64_t rem = 0;
    for (size_t i = n; i-- > 0;) {
        uint64_t t = rem * A334448_BASE + a[i];
        a[i] = (uint32_t)(t / m);
        rem = t % m;
    }
}
static void l_mul(uint32_t *a, size_t n, uint64_t m) {
    uint64_t carry = 0;
    for (size_t i = 0; i < n; i++) {
        uint64_t t = (uint64_t)a[i] * m + carry;
        a[i] = (uint32_t)(t % A334448_BASE);
        carry = t / A334448_BASE;
    }
}
static void l_add(uint32_t *a, const uint32_t *b, size_t n) {
    uint64_t carry = 0;
    for (size_t i = 0; i < n; i++) {
        uint64_t t = (uint64_t)a[i] + b[i] + carry;
        if (t >= A334448_BASE) { t -= A334448_BASE; carry = 1; } else carry = 0;
        a[i] = (uint32_t)t;
    }
}
static void l_sub(uint32_t *a, const uint32_t *b, size_t n) {
    int64_t borrow = 0;
    for (size_t i = 0; i < n; i++) {
        int64_t t = (int64_t)a[i] - (int64_t)b[i] - borrow;
        if (t < 0) { t += (int64_t)A334448_BASE; borrow = 1; } else borrow = 0;
        a[i] = (uint32_t)t;
    }
}
static bool l_zero(const uint32_t *a, size_t n) {
    for (size_t i = 0; i < n; i++) if (a[i]) return false;
    return true;
}
static bool l_eq(const uint32_t *a, const uint32_t *b, size_t n) {
    for (size_t i = 0; i < n; i++) if (a[i] != b[i]) return false;
    return true;
}
/* a /= m^e (truncating), batching small divisors */
static void l_divpow(uint32_t *a, size_t n, uint64_t m, unsigned e) {
    uint64_t dv = 1;
    for (unsigned i = 0; i < e; i++) {
        if (dv > 10000000000ULL / m) { l_div(a, n, dv); dv = 1; }
        dv *= m;
    }
    if (dv > 1) l_div(a, n, dv);
}

/* fixed point: g_L limbs, g_F fractional */
static void fx_one(uint32_t *x, uint32_t v) {
    memset(x, 0, g_L * sizeof(uint32_t));
    x[g_F] = v;
}
static void fx_mul(uint32_t *out, const uint32_t *a, const uint32_t *b) {
    size_t L = g_L;
    uint32_t *tmp = l_new(2 * L + 1);
    for (size_t i = 0; i < L; i++) {
        if (a[i] == 0) continue;
        uint64_t carry = 0;
        for (size_t j = 0; j < L; j++) {
            uint64_t t = (uint64_t)a[i] * b[j] + tmp[i + j] + carry;
            tmp[i + j] = (uint32_t)(t % A334448_BASE);
            carry = t / A334448_BASE;
        }
        size_t k = i + L;
        while (carry) {
            uint64_t t = (uint64_t)tmp[k] + carry;
            tmp[k] = (uint32_t)(t % A334448_BASE);
            carry = t / A334448_BASE;
            k++;
        }
    }
    memcpy(out, tmp + g_F, L * sizeof(uint32_t));
    free(tmp);
}
/* out = 1/x for x near 1 (Newton) */
static void fx_recip(const uint32_t *x, uint32_t *out) {
    size_t L = g_L;
    uint32_t *y = l_new(L), *t = l_new(L), *e = l_new(L), *ny = l_new(L);
    fx_one(y, 1);
    for (int it = 0; it < 200; it++) {
        fx_mul(t, x, y);
        fx_one(e, 2);
        l_sub(e, t, L);
        fx_mul(ny, y, e);
        bool same = l_eq(ny, y, L);
        memcpy(y, ny, L * sizeof(uint32_t));
        if (same) break;
    }
    memcpy(out, y, L * sizeof(uint32_t));
    free(y); free(t); free(e); free(ny);
}
/* out = sqrt(x) for x near 1 */
static void fx_sqrt(const uint32_t *x, uint32_t *out) {
    size_t L = g_L;
    uint32_t *y = l_new(L), *t = l_new(L), *e = l_new(L), *ny = l_new(L);
    fx_one(y, 1);
    for (int it = 0; it < 200; it++) {
        fx_mul(t, y, y);
        fx_mul(t, x, t);
        fx_one(e, 3);
        l_sub(e, t, L);
        fx_mul(ny, y, e);
        l_div(ny, L, 2);
        bool same = l_eq(ny, y, L);
        memcpy(y, ny, L * sizeof(uint32_t));
        if (same) break;
    }
    fx_mul(out, x, y);
    free(y); free(t); free(e); free(ny);
}

/* out = sum_{k>=0} (-1)^k m_k^-s, m_k = 2k+1 (odd) or k+1 */
static void alt_series(unsigned s, bool odd, uint32_t *out) {
    size_t L = g_L;
    uint32_t *pos = l_new(L), *neg = l_new(L), *term = l_new(L);
    for (size_t k = 0; k < g_N; k++) {
        uint64_t m = odd ? (uint64_t)(2 * k + 1) : (uint64_t)(k + 1);
        memcpy(term, g_W + k * L, L * sizeof(uint32_t));
        l_divpow(term, L, m, s);
        if (l_zero(term, L)) break;
        if (k & 1) l_add(neg, term, L); else l_add(pos, term, L);
    }
    l_sub(pos, neg, L);
    memcpy(out, pos, L * sizeof(uint32_t));
    free(pos); free(neg); free(term);
}

static void build_weights(void) {
    size_t N = g_N, F = g_F, L = g_L;
    size_t IL = N / 29 + 4;
    uint32_t *c = l_new(IL), *b = l_new(IL), *arr = l_new(F + IL);
    free(g_W);
    g_W = l_new(N * L);
    c[0] = 1;
    {
        size_t r = N;
        while (r > 0) {
            unsigned e = (unsigned)(r > 29 ? 29 : r);
            l_mul(c, IL, (uint64_t)1 << e);
            r -= e;
        }
    }
    b[0] = 1;
    for (size_t k = 0; k < N; k++) {
        l_sub(c, b, IL); /* c = sum_{j>k} binom(N,j) */
        memset(arr, 0, (F + IL) * sizeof(uint32_t));
        memcpy(arr + F, c, IL * sizeof(uint32_t));
        size_t r = N;
        while (r > 0) {
            unsigned e = (unsigned)(r > 29 ? 29 : r);
            l_div(arr, F + IL, (uint64_t)1 << e);
            r -= e;
        }
        memcpy(g_W + k * L, arr, L * sizeof(uint32_t));
        l_mul(b, IL, (uint64_t)(N - k));
        l_div(b, IL, (uint64_t)(k + 1));
    }
    free(c); free(b); free(arr);
}

static void compute_digits(size_t want) {
    g_F = (want + 30) / 9 + 1;
    g_L = g_F + 2;
    g_N = 30 * g_F + 20;
    size_t L = g_L;
    build_weights();

    /* primes up to M */
    bool *comp = (bool *)calloc(A334448_M + 1, sizeof(bool));
    unsigned primes[200];
    int np = 0;
    for (unsigned i = 2; i <= A334448_M; i++) {
        if (comp[i]) continue;
        primes[np++] = i;
        for (unsigned j = i * i; j <= A334448_M; j += i) comp[j] = true;
    }
    free(comp);

    uint32_t *C = l_new(L), *X = l_new(L), *Y = l_new(L), *y = l_new(L);
    uint32_t *u = l_new(L), *ru = l_new(L), *R = l_new(L);
    fx_one(C, 1);
    unsigned j = 0;
    for (unsigned s = 4;; s *= 2, j++) {
        /* X = beta(s) * Prod_{odd p<=M} (1 - chi(p) p^-s) */
        alt_series(s, true, X);
        for (int i = 1; i < np; i++) {
            unsigned p = primes[i];
            memcpy(y, X, L * sizeof(uint32_t));
            l_divpow(y, L, p, s);
            if (p % 4 == 1) l_sub(X, y, L); else l_add(X, y, L);
        }
        /* Y = eta(s)*(1-2^-s)*Prod_{odd p<=M}(1-p^-s) / (1-2^(1-s)) */
        alt_series(s, false, Y);
        for (int i = 0; i < np; i++) {
            memcpy(y, Y, L * sizeof(uint32_t));
            l_divpow(y, L, primes[i], s);
            l_sub(Y, y, L);
        }
        fx_one(u, 1);
        fx_one(y, 1);
        l_divpow(y, L, 2, s - 1);
        l_sub(u, y, L);
        fx_recip(u, ru);
        fx_mul(Y, Y, ru);
        fx_recip(Y, ru);
        fx_mul(R, X, ru);
        for (unsigned i = 0; i <= j; i++) fx_sqrt(R, R);
        fx_mul(C, C, R);
        if ((size_t)6 * s >= 9 * g_F + 9) break;
    }
    /* small primes 3 mod 4 */
    for (int i = 0; i < np; i++) {
        if (primes[i] % 4 != 3) continue;
        memcpy(y, C, L * sizeof(uint32_t));
        l_divpow(y, L, primes[i], 4);
        l_sub(C, y, L);
    }

    unsigned char *dg = (unsigned char *)malloc(want + 1);
    size_t pos = 0;
    for (size_t i = g_F; i-- > 0 && pos < want;) {
        uint32_t v = C[i];
        unsigned char tmp[9];
        for (int k = 8; k >= 0; k--) { tmp[k] = (unsigned char)(v % 10); v /= 10; }
        for (int k = 0; k < 9 && pos < want; k++) dg[pos++] = tmp[k];
    }
    free(g_dig);
    g_dig = dg;
    g_dig_len = pos;

    free(C); free(X); free(Y); free(y); free(u); free(ru); free(R);
    free(g_W);
    g_W = 0;
}

seq_t a(idx_t n) {
    size_t need = (size_t)n + 1;
    if (need > g_dig_len) {
        size_t want = need;
        if (want < 2 * g_dig_len) want = 2 * g_dig_len;
        if (want < 64) want = 64;
        compute_digits(want);
    }
    return (seq_t)(int)g_dig[n];
}
