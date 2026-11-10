/*
 * A248136  Least positive integer m such that m + n divides D(m) + D(n),
 *          where D(.) is given by A001850.
 * https://oeis.org/A248136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m >= 1 with (m+n) | D(m) + D(n), D = A001850; found
 *            by search over m. Each prime p | m+n is first tested with the
 *            Lucas congruence D(p*q+r) == D(q)*D(r) (mod p) and the
 *            reflection D(p-1-j) == D(j) (mod p). If m+n is not squarefree,
 *            the survivors are then checked exactly modulo m+n
 * Range:     u8 n <= 7, u16 n <= 1483, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9029 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned __int128 a248136_u128;

static uint64_t a248136_mul(uint64_t a, uint64_t b, uint64_t M) {
    return (uint64_t)((a248136_u128)a * b % M);
}

static uint64_t a248136_inv(uint64_t a, uint64_t M) {
    __int128 r0 = (__int128)M, r1 = (__int128)(a % M);
    __int128 t0 = 0, t1 = 1;
    while (r1 != 0) {
        __int128 q = r0 / r1;
        __int128 r2 = r0 - q * r1; r0 = r1; r1 = r2;
        __int128 t2 = t0 - q * t1; t0 = t1; t1 = t2;
    }
    if (t0 < 0) t0 += (__int128)M;
    return (uint64_t)t0;
}

static uint64_t a248136_powm(uint64_t b, uint64_t e, uint64_t M) {
    uint64_t r = 1 % M;
    b %= M;
    while (e) {
        if (e & 1) r = a248136_mul(r, b, M);
        b = a248136_mul(b, b, M);
        e >>= 1;
    }
    return r;
}

/* exact D(N) mod M, tracking p-adic parts for primes p | M */
static uint64_t a248136_Dexact(uint64_t N, uint64_t M, const uint64_t *ps, int np) {
    int e[24];
    for (int i = 0; i < np; i++) e[i] = 0;
    uint64_t u = 1 % M;
    uint64_t sum = 0;
    for (uint64_t k = 0; ; k++) {
        uint64_t v = u;
        for (int i = 0; i < np; i++)
            if (e[i] > 0) v = a248136_mul(v, a248136_powm(ps[i], (uint64_t)e[i], M), M);
        sum += v;
        if (sum >= M) sum -= M;
        if (k == N) break;
        uint64_t x1 = N - k, x2 = N + k + 1, d = k + 1;
        for (int i = 0; i < np; i++) {
            uint64_t p = ps[i];
            while (x1 % p == 0) { x1 /= p; e[i]++; }
            while (x2 % p == 0) { x2 /= p; e[i]++; }
            while (d % p == 0) { d /= p; e[i] -= 2; }
        }
        u = a248136_mul(u, x1 % M, M);
        u = a248136_mul(u, x2 % M, M);
        uint64_t di = a248136_inv(d % M, M);
        u = a248136_mul(u, di, M);
        u = a248136_mul(u, di, M);
    }
    return sum;
}

/* ---- table of exact D(j) as 32-bit limb big integers ---- */
static uint32_t *a248136_TL = NULL;
static size_t *a248136_TOFF = NULL, *a248136_TLEN = NULL;
static uint64_t a248136_TCNT = 0;
static size_t a248136_TCAPL = 0, a248136_TCAPN = 0, a248136_TUSED = 0;

static void a248136_push(const uint32_t *v, size_t len) {
    if (a248136_TUSED + len > a248136_TCAPL) {
        size_t nc = a248136_TCAPL ? a248136_TCAPL : 1024;
        while (nc < a248136_TUSED + len) nc *= 2;
        a248136_TL = (uint32_t *)realloc(a248136_TL, nc * sizeof(uint32_t));
        a248136_TCAPL = nc;
    }
    if (a248136_TCNT + 1 > a248136_TCAPN) {
        size_t nc = a248136_TCAPN ? a248136_TCAPN * 2 : 256;
        a248136_TOFF = (size_t *)realloc(a248136_TOFF, nc * sizeof(size_t));
        a248136_TLEN = (size_t *)realloc(a248136_TLEN, nc * sizeof(size_t));
        a248136_TCAPN = nc;
    }
    memcpy(a248136_TL + a248136_TUSED, v, len * sizeof(uint32_t));
    a248136_TOFF[a248136_TCNT] = a248136_TUSED;
    a248136_TLEN[a248136_TCNT] = len;
    a248136_TUSED += len;
    a248136_TCNT++;
}

static void a248136_tab_ensure(uint64_t upto) {
    if (a248136_TCNT == 0) {
        uint32_t one = 1, three = 3;
        a248136_push(&one, 1);
        a248136_push(&three, 1);
    }
    while (a248136_TCNT <= upto) {
        uint64_t j = a248136_TCNT;
        const uint32_t *A = a248136_TL + a248136_TOFF[j - 1];
        size_t la = a248136_TLEN[j - 1];
        const uint32_t *Bv = a248136_TL + a248136_TOFF[j - 2];
        size_t lb = a248136_TLEN[j - 2];
        size_t L = la + 1;
        uint32_t *P = (uint32_t *)malloc(L * sizeof(uint32_t));
        uint32_t *Q = (uint32_t *)malloc(L * sizeof(uint32_t));
        uint64_t ca = 6 * j - 3, cb = j - 1, carry = 0;
        for (size_t i = 0; i < la; i++) {
            uint64_t t = (uint64_t)A[i] * ca + carry;
            P[i] = (uint32_t)t; carry = t >> 32;
        }
        P[la] = (uint32_t)carry;
        carry = 0;
        for (size_t i = 0; i < L; i++) {
            uint64_t t = (i < lb ? (uint64_t)Bv[i] * cb : 0) + carry;
            Q[i] = (uint32_t)t; carry = t >> 32;
        }
        int64_t bor = 0;
        for (size_t i = 0; i < L; i++) {
            int64_t t = (int64_t)P[i] - (int64_t)Q[i] - bor;
            if (t < 0) { t += (int64_t)1 << 32; bor = 1; } else bor = 0;
            P[i] = (uint32_t)t;
        }
        uint64_t r = 0;
        for (size_t i = L; i-- > 0; ) {
            uint64_t cur = (r << 32) | P[i];
            P[i] = (uint32_t)(cur / j);
            r = cur % j;
        }
        while (L > 1 && P[L - 1] == 0) L--;
        a248136_push(P, L);
        free(P); free(Q);
    }
}

static uint64_t a248136_bigmod(const uint32_t *l, size_t len, uint64_t p) {
    uint64_t r = 0;
    if (p <= 0xFFFFFFFFull) {
        for (size_t i = len; i-- > 0; ) r = ((r << 32) | l[i]) % p;
    } else {
        for (size_t i = len; i-- > 0; )
            r = (uint64_t)((((a248136_u128)r << 32) | l[i]) % p);
    }
    return r;
}

/* D(d) mod p for 0 <= d < p, p prime */
static uint64_t a248136_Dsmall(uint64_t d, uint64_t p) {
    uint64_t dd = d;
    if (p - 1 - d < dd) dd = p - 1 - d;
    if (dd < a248136_TCNT)
        return a248136_bigmod(a248136_TL + a248136_TOFF[dd], a248136_TLEN[dd], p);
    uint64_t x0 = 1 % p, x1 = 3 % p;
    for (uint64_t j = 2; j <= dd; j++) {
        uint64_t t = a248136_mul((6 * j - 3) % p, x1, p);
        uint64_t s = a248136_mul((p - (j - 1) % p) % p, x0, p);
        t += s; if (t >= p) t -= p;
        t = a248136_mul(t, a248136_inv(j % p, p), p);
        x0 = x1; x1 = t;
    }
    return x1;
}

/* D(x) mod p via Lucas property */
static uint64_t a248136_Dmodp(uint64_t x, uint64_t p) {
    uint64_t r = 1 % p;
    while (x > 0) {
        uint64_t d = x % p;
        x /= p;
        r = a248136_mul(r, a248136_Dsmall(d, p), p);
        if (r == 0) return 0;
    }
    return r;
}

/* ---- primes for segmented factoring ---- */
static uint32_t *a248136_PR = NULL;
static size_t a248136_NPR = 0;
static uint64_t a248136_PLIM = 0;

static void a248136_primes_build(uint64_t L) {
    char *s = (char *)calloc((size_t)L + 1, 1);
    free(a248136_PR);
    a248136_PR = (uint32_t *)malloc(((size_t)L / 2 + 16) * sizeof(uint32_t));
    a248136_NPR = 0;
    for (uint64_t i = 2; i <= L; i++) {
        if (!s[i]) {
            a248136_PR[a248136_NPR++] = (uint32_t)i;
            for (uint64_t k = i * i; k <= L; k += i) s[k] = 1;
        }
    }
    free(s);
    a248136_PLIM = L;
}

#define A248136_B 4096
static uint64_t a248136_rem[A248136_B];
static uint64_t a248136_fac[A248136_B * 16];
static int a248136_cnt[A248136_B];
static bool a248136_sqf[A248136_B];

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    a248136_tab_ensure(N);
    for (uint64_t M0 = N + 1; ; M0 += A248136_B) {
        uint64_t M1 = M0 + A248136_B;
        while ((a248136_u128)a248136_PLIM * a248136_PLIM < M1)
            a248136_primes_build(a248136_PLIM * 2 + 1024);
        for (int i = 0; i < A248136_B; i++) {
            a248136_rem[i] = M0 + (uint64_t)i;
            a248136_cnt[i] = 0;
            a248136_sqf[i] = true;
        }
        for (size_t k = 0; k < a248136_NPR; k++) {
            uint64_t p = a248136_PR[k];
            if (p * p >= M1) break;
            uint64_t st = ((M0 + p - 1) / p) * p;
            for (uint64_t x = st; x < M1; x += p) {
                int i = (int)(x - M0);
                a248136_fac[i * 16 + a248136_cnt[i]++] = p;
                int e = 0;
                while (a248136_rem[i] % p == 0) { a248136_rem[i] /= p; e++; }
                if (e >= 2) a248136_sqf[i] = false;
            }
        }
        for (int i = 0; i < A248136_B; i++) {
            if (a248136_rem[i] > 1)
                a248136_fac[i * 16 + a248136_cnt[i]++] = a248136_rem[i];
        }
        for (int i = 0; i < A248136_B; i++) {
            uint64_t M = M0 + (uint64_t)i;
            uint64_t m = M - N;
            const uint64_t *ps = a248136_fac + i * 16;
            int np = a248136_cnt[i];
            bool ok = true;
            for (int k = 0; k < np && ok; k++) {
                uint64_t p = ps[k];
                uint64_t s = a248136_Dmodp(m, p) + a248136_Dmodp(N, p);
                if (s % p != 0) ok = false;
            }
            if (!ok) continue;
            if (a248136_sqf[i]) return (seq_t)m;
            uint64_t s = a248136_Dexact(m, M, ps, np) + a248136_Dexact(N, M, ps, np);
            if (s % M == 0) return (seq_t)m;
        }
    }
}
