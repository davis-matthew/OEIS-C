/*
 * A373763  Numbers k that are neither primes nor squares of primes such that
 *          A006134(k) - A102283(k) is divisible by k.
 * https://oeis.org/A373763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: k = 1, or k composite and not a prime square with
 *            Sum_{j=0..k-1} binomial(2j,j) == A102283(k) (mod k). Mod each
 *            prime p | k this is tested by a Lucas-based sieve (S(p*q) ==
 *            (p/3)*S(q) mod p, S(n) = Sum_{j<n} C(2j,j)); mod p^e, e >= 2, by
 *            resumable running sums of C(2j,j) in Montgomery ari...
 * Range:     u8 n <= 7, u16 n <= 83, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 978 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef unsigned __int128 a373763_u128;

typedef struct {
    uint64_t p, M, minv, N, D, T, j, rj, rx;
    long v;
    int E;
    uint64_t pwR[64];
} a373763_pass_t;

static uint64_t a373763_Amod(uint64_t k, uint64_t m) {
    uint64_t r = k % 3;
    if (r == 0) return 0;
    if (r == 1) return 1 % m;
    return (m - 1) % m;
}

static inline uint64_t a373763_redc(a373763_u128 t, uint64_t M, uint64_t minv) {
    uint64_t m = (uint64_t)t * minv;
    a373763_u128 u = (t + (a373763_u128)m * M) >> 64;
    uint64_t r = (uint64_t)u;
    if (r >= M) r -= M;
    return r;
}

static uint64_t a373763_inv(uint64_t a, uint64_t m) {
    __int128 r0 = (__int128)m, r1 = (__int128)(a % m), t0 = 0, t1 = 1;
    while (r1 != 0) {
        __int128 q = r0 / r1, tmp;
        tmp = r0 - q * r1; r0 = r1; r1 = tmp;
        tmp = t0 - q * t1; t0 = t1; t1 = tmp;
    }
    if (t0 < 0) t0 += (__int128)m;
    return (uint64_t)t0;
}

static a373763_pass_t **a373763_passes = 0;
static size_t a373763_passcap = 0;

static a373763_pass_t *a373763_get_pass(uint64_t p) {
    if (p >= a373763_passcap) {
        size_t nc = a373763_passcap ? a373763_passcap : 64;
        while (nc <= p) nc *= 2;
        a373763_passes = (a373763_pass_t **)realloc(a373763_passes, nc * sizeof(a373763_pass_t *));
        for (size_t i = a373763_passcap; i < nc; i++) a373763_passes[i] = 0;
        a373763_passcap = nc;
    }
    if (!a373763_passes[p]) {
        a373763_pass_t *s = (a373763_pass_t *)calloc(1, sizeof(a373763_pass_t));
        s->p = p;
        s->N = s->D = s->T = 1;
        s->j = 0;
        s->v = 0;
        if (p != 2) {
            uint64_t M = 1;
            int E = 0;
            while (M <= (UINT64_C(1) << 62) / p) { M *= p; E++; }
            s->M = M;
            s->E = E;
            uint64_t inv = M;
            for (int i = 0; i < 6; i++) inv *= 2 - M * inv;
            s->minv = (uint64_t)0 - inv;
            s->pwR[0] = (uint64_t)((((a373763_u128)1) << 64) % M);
            for (int i = 1; i < E && i < 64; i++)
                s->pwR[i] = (uint64_t)(((a373763_u128)s->pwR[i - 1] * p) % M);
            s->rj = 1 % p;
            s->rx = 1 % p;
        }
        a373763_passes[p] = s;
    }
    return a373763_passes[p];
}

/* advance so that the state holds Sum_{j<k} C(2j,j); return it mod p^e */
static uint64_t a373763_S(a373763_pass_t *s, uint64_t k, int e) {
    uint64_t p = s->p;
    if (p == 2) {
        while (s->j + 1 < k) {
            uint64_t j = ++s->j;
            uint64_t x = 2 * j - 1, y = j;
            s->v += 1;
            while (!(y & 1)) { y >>= 1; s->v--; }
            s->N *= x;
            s->D *= y;
            s->T = s->T * y + (s->v < 64 ? (s->N << s->v) : 0);
        }
        uint64_t D = s->D, inv = D;
        for (int i = 0; i < 6; i++) inv *= 2 - D * inv;
        uint64_t S = s->T * inv;
        if (e < 64) S &= ((UINT64_C(1) << e) - 1);
        return S;
    }
    uint64_t M = s->M, minv = s->minv;
    int E = s->E;
    while (s->j + 1 < k) {
        uint64_t j = ++s->j;
        uint64_t x = 4 * j - 2, y = j;
        if (s->rx == 0) { do { x /= p; s->v++; } while (x % p == 0); }
        if (s->rj == 0) { do { y /= p; s->v--; } while (y % p == 0); }
        s->N = a373763_redc((a373763_u128)s->N * x, M, minv);
        s->D = a373763_redc((a373763_u128)s->D * y, M, minv);
        s->T = a373763_redc((a373763_u128)s->T * y, M, minv);
        if (s->v < E) {
            s->T += a373763_redc((a373763_u128)s->pwR[s->v] * s->N, M, minv);
            if (s->T >= M) s->T -= M;
        }
        s->rj++; if (s->rj == p) s->rj = 0;
        s->rx += 2; if (s->rx >= p) s->rx -= p;
    }
    uint64_t S = (uint64_t)(((a373763_u128)s->T * a373763_inv(s->D, M)) % M);
    uint64_t pe = 1;
    for (int i = 0; i < e; i++) pe *= p;
    return S % pe;
}

static uint64_t *a373763_terms = 0;
static size_t a373763_cnt = 0, a373763_cap = 0;
static uint64_t a373763_Kdone = 0;

static void a373763_push(uint64_t k) {
    if (a373763_cnt == a373763_cap) {
        a373763_cap = a373763_cap ? a373763_cap * 2 : 64;
        a373763_terms = (uint64_t *)realloc(a373763_terms, a373763_cap * sizeof(uint64_t));
    }
    a373763_terms[a373763_cnt++] = k;
}

#define A373763_GET(b, i) (((b)[(i) >> 6] >> ((i) & 63)) & 1)
#define A373763_SET(b, i) ((b)[(i) >> 6] |= (UINT64_C(1) << ((i) & 63)))

static void a373763_round(void) {
    uint64_t K0 = a373763_Kdone;
    uint64_t K = K0 * 2;
    if (K < 65536) K = 65536;
    size_t words = (size_t)(K / 64 + 1);
    uint64_t *comp = (uint64_t *)calloc(words, sizeof(uint64_t));
    uint64_t *bad = (uint64_t *)calloc(words, sizeof(uint64_t));
    A373763_SET(comp, 0); A373763_SET(comp, 1);
    for (uint64_t i = 2; i * i <= K; i++)
        if (!A373763_GET(comp, i))
            for (uint64_t j = i * i; j <= K; j += i) A373763_SET(comp, j);
    uint64_t sq = 1;
    while ((sq + 1) * (sq + 1) <= K) sq++;
    size_t Lmax = (size_t)sq + 2;
    uint64_t *cc = (uint64_t *)malloc(Lmax * sizeof(uint64_t));
    uint64_t *iv = (uint64_t *)malloc(Lmax * sizeof(uint64_t));

    for (uint64_t p = 2; p <= K / 2; p++) {
        if (A373763_GET(comp, p)) continue;
        uint64_t Q = K / p;
        uint64_t L = (p - 1 < Q) ? p - 1 : Q;
        cc[0] = 1;
        iv[1] = 1;
        for (uint64_t d = 1; d <= L; d++) {
            if (d > 1) iv[d] = (p - (p / d) * iv[p % d] % p) % p;
            cc[d] = cc[d - 1] * ((4 * d - 2) % p) % p * iv[d] % p;
        }
        uint64_t F;
        if (p == 2) F = 1;
        else if (p == 3) F = 0;
        else F = (p % 3 == 1) ? 1 : p - 1;
        uint64_t g = 0;
        for (uint64_t q = 0; q <= Q; q++) {
            if (q >= 2 && p * q >= K0) {
                uint64_t s = F * g % p;
                if (s != a373763_Amod(p * q, p)) A373763_SET(bad, p * q);
            }
            if (q == Q) break;
            uint64_t c;
            if (q < p) c = cc[q];
            else {
                c = 1;
                uint64_t t = q;
                while (t) {
                    c = c * cc[t % p] % p;
                    if (!c) break;
                    t /= p;
                }
            }
            g += c;
            if (g >= p) g -= p;
        }
    }

    uint64_t *pl = (uint64_t *)malloc(Lmax * sizeof(uint64_t));
    size_t npl = 0;
    for (uint64_t i = 2; i <= sq; i++) if (!A373763_GET(comp, i)) pl[npl++] = i;

    for (uint64_t k = K0; k <= K; k++) {
        if (!A373763_GET(comp, k)) continue;   /* prime */
        if (A373763_GET(bad, k)) continue;
        uint64_t ps[20];
        int es[20];
        int np = 0;
        uint64_t rem = k;
        for (size_t i = 0; i < npl && pl[i] * pl[i] <= rem; i++) {
            if (rem % pl[i] == 0) {
                int e = 0;
                while (rem % pl[i] == 0) { rem /= pl[i]; e++; }
                ps[np] = pl[i]; es[np] = e; np++;
            }
        }
        if (rem > 1) { ps[np] = rem; es[np] = 1; np++; }
        if (np == 1 && es[0] <= 2) continue;
        bool ok = true;
        for (int i = 0; i < np && ok; i++) {
            if (es[i] >= 2) {
                a373763_pass_t *s = a373763_get_pass(ps[i]);
                uint64_t pe = 1;
                for (int t = 0; t < es[i]; t++) pe *= ps[i];
                if (a373763_S(s, k, es[i]) != a373763_Amod(k, pe)) ok = false;
            }
        }
        if (ok) a373763_push(k);
    }
    free(pl);
    free(cc);
    free(iv);
    free(comp);
    free(bad);
    a373763_Kdone = K + 1;
}

seq_t a(idx_t n) {
    if (a373763_Kdone == 0) {
        a373763_push(1);
        a373763_Kdone = 2;
    }
    while (a373763_cnt < (size_t)n) a373763_round();
    return (seq_t)a373763_terms[n - 1];
}
