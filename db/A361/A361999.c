/*
 * A361999  a(n) is the smallest k such that A361902(k) = n, or -1 if no such
 *          k exists.
 * https://oeis.org/A361999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   search: a(n) = least k such that k+F(j) is not prime for all 0
 *            <= j < n and k+F(n) is prime (F = A000045); a(2) = -1 since F(1)
 *            = F(2). Uses a block prime sieve for the small-j tests, a
 *            residue sieve on k+F(n), and Miller-Rabin
 * Range:     i8 n <= 9, i16 n <= 60, i32 n <= 500, i64 n <= 500,
 *            i128 n <= 500, big any n
 * Validated: standard, pass, 67 terms through n = 491 (2026-11-10)
 * Helpers:   ctz_u64, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_u64.h"
#include "../utils/ctz_u64.h"

#define A361999_LIM (1u << 20)
#define A361999_B ((uint64_t)1 << 20)
#define A361999_NT 100

static uint32_t *A361999_pr = NULL;
static size_t A361999_npr = 0;

static void A361999_init(void) {
    if (A361999_pr) return;
    unsigned char *c = (unsigned char *)calloc(A361999_LIM, 1);
    size_t cnt = 0;
    for (uint32_t i = 2; i < A361999_LIM; i++) {
        if (c[i]) continue;
        cnt++;
        for (uint64_t q = (uint64_t)i * i; q < A361999_LIM; q += i) c[q] = 1;
    }
    uint32_t *p = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    size_t t = 0;
    for (uint32_t i = 2; i < A361999_LIM; i++) if (!c[i]) p[t++] = i;
    free(c);
    A361999_npr = cnt;
    A361999_pr = p;
}

static uint32_t A361999_fibmod(uint64_t n, uint32_t p) {
    uint64_t x = 0, y = 1;
    int hb = 63;
    while (hb >= 0 && !((n >> hb) & 1u)) hb--;
    for (int i = hb; i >= 0; i--) {
        uint64_t c = x * ((2 * y + p - x) % p) % p;
        uint64_t d = (x * x + y * y) % p;
        if ((n >> i) & 1u) { x = d; y = (c + d) % p; }
        else { x = c; y = d; }
    }
    return (uint32_t)x;
}

static int A361999_cmp(const uint32_t *x, const uint32_t *y, int s) {
    for (int i = s - 1; i >= 0; i--)
        if (x[i] != y[i]) return x[i] < y[i] ? -1 : 1;
    return 0;
}

static void A361999_sub(uint32_t *x, const uint32_t *y, int s) {
    uint64_t borrow = 0;
    for (int i = 0; i < s; i++) {
        uint64_t d = (uint64_t)x[i] - y[i] - borrow;
        x[i] = (uint32_t)d;
        borrow = (d >> 32) & 1u;
    }
}

static void A361999_addmod(uint32_t *x, const uint32_t *y, const uint32_t *m, int s) {
    uint64_t c = 0;
    for (int i = 0; i < s; i++) {
        uint64_t v = (uint64_t)x[i] + y[i] + c;
        x[i] = (uint32_t)v;
        c = v >> 32;
    }
    if (c || A361999_cmp(x, m, s) >= 0) A361999_sub(x, m, s);
}

static void A361999_montmul(uint32_t *r, const uint32_t *x, const uint32_t *y,
                            const uint32_t *m, uint32_t minv, int s, uint32_t *t) {
    memset(t, 0, sizeof(uint32_t) * (size_t)(s + 2));
    for (int i = 0; i < s; i++) {
        uint64_t C = 0;
        uint64_t bi = y[i];
        for (int j = 0; j < s; j++) {
            uint64_t v = (uint64_t)t[j] + (uint64_t)x[j] * bi + C;
            t[j] = (uint32_t)v;
            C = v >> 32;
        }
        uint64_t v = (uint64_t)t[s] + C;
        t[s] = (uint32_t)v;
        t[s + 1] = (uint32_t)(v >> 32);
        uint64_t mm = (uint32_t)(t[0] * minv);
        v = (uint64_t)t[0] + mm * m[0];
        C = v >> 32;
        for (int j = 1; j < s; j++) {
            v = (uint64_t)t[j] + mm * m[j] + C;
            t[j - 1] = (uint32_t)v;
            C = v >> 32;
        }
        v = (uint64_t)t[s] + C;
        t[s - 1] = (uint32_t)v;
        t[s] = t[s + 1] + (uint32_t)(v >> 32);
    }
    if (t[s] != 0 || A361999_cmp(t, m, s) >= 0) A361999_sub(t, m, s);
    memcpy(r, t, sizeof(uint32_t) * (size_t)s);
}

/* strong probable-prime test on the first nb bases, m odd > 37, s limbs */
static bool A361999_mr_big(const uint32_t *m, int s, uint32_t *ws, int nb) {
    static const uint32_t bases[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    if (!(m[0] & 1u)) return false;
    if (nb > 12) nb = 12;
    uint32_t *t = ws;
    uint32_t *oneR = t + (s + 2);
    uint32_t *mone = oneR + s;
    uint32_t *aR = mone + s;
    uint32_t *x = aR + s;

    uint32_t inv = m[0];
    for (int i = 0; i < 5; i++) inv *= 2u - m[0] * inv;
    uint32_t minv = (uint32_t)(0u - inv);

    memset(oneR, 0, sizeof(uint32_t) * (size_t)s);
    oneR[0] = 1;
    for (int b = 0; b < 32 * s; b++) {
        uint64_t c = 0;
        for (int i = 0; i < s; i++) {
            uint64_t v = ((uint64_t)oneR[i] << 1) | c;
            oneR[i] = (uint32_t)v;
            c = v >> 32;
        }
        if (c || A361999_cmp(oneR, m, s) >= 0) A361999_sub(oneR, m, s);
    }
    memcpy(mone, m, sizeof(uint32_t) * (size_t)s);
    A361999_sub(mone, oneR, s);

    int top = 32 * (s - 1);
    { uint32_t h = m[s - 1]; int hb = -1; while (h) { hb++; h >>= 1; } top += hb; }
    int r = 1;
    while (!((m[r / 32] >> (r % 32)) & 1u)) r++;

    for (int bi = 0; bi < nb; bi++) {
        memset(aR, 0, sizeof(uint32_t) * (size_t)s);
        for (uint32_t q = 0; q < bases[bi]; q++) A361999_addmod(aR, oneR, m, s);
        memcpy(x, oneR, sizeof(uint32_t) * (size_t)s);
        for (int b = top; b >= r; b--) {
            A361999_montmul(x, x, x, m, minv, s, t);
            if ((m[b / 32] >> (b % 32)) & 1u) A361999_montmul(x, x, aR, m, minv, s, t);
        }
        if (A361999_cmp(x, oneR, s) == 0 || A361999_cmp(x, mone, s) == 0) continue;
        bool ok = false;
        for (int i = 1; i < r; i++) {
            A361999_montmul(x, x, x, m, minv, s, t);
            if (A361999_cmp(x, mone, s) == 0) { ok = true; break; }
            if (A361999_cmp(x, oneR, s) == 0) break;
        }
        if (!ok) return false;
    }
    return true;
}

static int A361999_mknum(uint32_t *num, const uint32_t *f, int L, uint64_t k) {
    uint64_t c = k;
    for (int i = 0; i < L; i++) {
        uint64_t w = (uint64_t)f[i] + (c & 0xFFFFFFFFu);
        c >>= 32;
        c += w >> 32;
        num[i] = (uint32_t)w;
    }
    int s = L;
    while (s > 1 && num[s - 1] == 0) s--;
    return s;
}

static void A361999_andsh(uint64_t *S, const uint64_t *NP, uint64_t off, size_t BW, int inv) {
    size_t q0 = (size_t)(off >> 6);
    unsigned r = (unsigned)(off & 63);
    for (size_t w = 0; w < BW; w++) {
        size_t q = q0 + w;
        uint64_t v = r ? ((NP[q] >> r) | (NP[q + 1] << (64 - r))) : NP[q];
        S[w] &= inv ? ~v : v;
    }
}

seq_t a(idx_t n) {
    if (n == 2) return (seq_t)-1;
    A361999_init();
    const uint32_t *pr = A361999_pr;
    const size_t npr = A361999_npr;
    const uint64_t B = A361999_B;
    const size_t BW = (size_t)(B / 64);
    const int NT = A361999_NT;

    uint64_t F64[94];
    F64[0] = 0; F64[1] = 1;
    for (int j = 2; j < 94; j++) F64[j] = F64[j - 1] + F64[j - 2];
    int J = 2;
    while (J + 1 < 94 && F64[J + 1] <= B) J++;

    size_t N = (size_t)n;
    bool small = (N <= (size_t)J);
    size_t WW = (size_t)((B + F64[J]) / 64) + 2;
    uint64_t *NPb = (uint64_t *)malloc(WW * sizeof(uint64_t));
    uint64_t *S = (uint64_t *)malloc(BW * sizeof(uint64_t));
    uint64_t *T = (uint64_t *)malloc(BW * sizeof(uint64_t));
    int jl = small ? (int)N - 1 : J;

    uint32_t *fnmod = NULL;
    uint16_t *Fm = NULL;
    uint32_t *Fb = NULL, *num = NULL, *ws = NULL;
    int L = (int)(N * 7 / 320) + 4;
    if (!small) {
        fnmod = (uint32_t *)malloc(npr * sizeof(uint32_t));
        for (size_t t = 0; t < npr; t++) fnmod[t] = A361999_fibmod((uint64_t)N, pr[t]);
        Fm = (uint16_t *)malloc((N + 1) * (size_t)NT * sizeof(uint16_t));
        for (int i = 0; i < NT; i++) {
            uint32_t p = pr[i];
            Fm[i] = 0;
            Fm[(size_t)NT + i] = (uint16_t)(1 % p);
            for (size_t j = 2; j <= N; j++)
                Fm[j * NT + i] = (uint16_t)(((uint32_t)Fm[(j - 1) * NT + i] + Fm[(j - 2) * NT + i]) % p);
        }
        if (N >= 93) {
            Fb = (uint32_t *)calloc((N + 1) * (size_t)L, sizeof(uint32_t));
            Fb[(size_t)L] = 1;
            for (size_t j = 2; j <= N; j++) {
                uint64_t c = 0;
                for (int i = 0; i < L; i++) {
                    uint64_t v = (uint64_t)Fb[(j - 1) * L + i] + Fb[(j - 2) * L + i] + c;
                    Fb[j * L + i] = (uint32_t)v;
                    c = v >> 32;
                }
            }
        }
        num = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)L);
        ws = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)(6 * L + 8));
    }

    uint64_t result = 0;
    bool found = false;
    uint32_t kmod[A361999_NT];
    for (uint64_t K0 = 0; !found; K0 += B) {
        memset(NPb, 0, WW * sizeof(uint64_t));
        uint64_t span = (uint64_t)WW * 64;
        uint64_t hi = K0 + span;
        if (K0 == 0) NPb[0] |= 3u;
        for (size_t t = 0; t < npr; t++) {
            uint64_t p = pr[t];
            if (p * p >= hi) break;
            uint64_t st = p * p;
            if (st < K0) st = ((K0 + p - 1) / p) * p;
            for (uint64_t v = st - K0; v < span; v += p) NPb[v >> 6] |= (uint64_t)1 << (v & 63);
        }
        for (size_t w = 0; w < BW; w++) S[w] = ~(uint64_t)0;
        for (int j = 0; j <= jl; j++) {
            if (j == 2) continue;
            A361999_andsh(S, NPb, F64[j], BW, 0);
        }
        if (small) {
            A361999_andsh(S, NPb, F64[N], BW, 1);
            for (size_t w = 0; w < BW; w++) {
                if (S[w]) {
                    result = K0 + (uint64_t)w * 64 + (uint64_t)ctz_u64(S[w]);
                    found = true;
                    break;
                }
            }
            continue;
        }

        memcpy(T, S, BW * sizeof(uint64_t));
        for (size_t t = 0; t < npr; t++) {
            uint64_t p = pr[t];
            uint64_t r = (2 * p - fnmod[t] - K0 % p) % p;
            for (uint64_t i = r; i < B; i += p) T[i >> 6] &= ~((uint64_t)1 << (i & 63));
        }

        for (size_t w = 0; w < BW && !found; w++) {
            uint64_t bits = S[w] & T[w];
            while (bits) {
                int b = ctz_u64(bits);
                bits &= bits - 1;
                uint64_t k = K0 + (uint64_t)w * 64 + (uint64_t)b;
                bool ok;
                int s;
                if (N <= 92) ok = is_prime_u64(k + F64[N]);
                else {
                    s = A361999_mknum(num, Fb + N * L, L, k);
                    ok = A361999_mr_big(num, s, ws, 1);
                }
                if (!ok) continue;
                for (int i = 0; i < NT; i++) kmod[i] = (uint32_t)(k % pr[i]);
                bool rej = false;
                for (size_t j = (size_t)J + 1; j < N && !rej; j++) {
                    const uint16_t *fm = Fm + j * NT;
                    bool comp = false;
                    for (int i = 0; i < NT; i++) {
                        uint32_t sm = kmod[i] + fm[i];
                        if (sm >= pr[i]) sm -= pr[i];
                        if (sm == 0) { comp = true; break; }
                    }
                    if (comp) continue;
                    if (j <= 92) rej = is_prime_u64(k + F64[j]);
                    else {
                        s = A361999_mknum(num, Fb + j * L, L, k);
                        rej = A361999_mr_big(num, s, ws, 2);
                    }
                }
                if (rej) continue;
                if (N > 92) {
                    s = A361999_mknum(num, Fb + N * L, L, k);
                    if (!A361999_mr_big(num, s, ws, 12)) continue;
                }
                result = k;
                found = true;
                break;
            }
        }
    }

    free(NPb); free(S); free(T);
    free(fnmod); free(Fm); free(Fb); free(num); free(ws);
    return (seq_t)(idx_t)result;
}
